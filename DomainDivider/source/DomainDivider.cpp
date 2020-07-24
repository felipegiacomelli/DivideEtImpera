#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include "DivideEtImpera/DomainDivider/LocalGridDataBuilder.hpp"
#include <cgnslib.h>

SubdomainDataPtr DomainDivider::divideDomain(GridDataPtr gridData, DomainPartitioner domainPartitioner) {
    if (this->world.size() == 1) {
        this->verticesOfWells = this->buildVerticesOfWells(gridData);
        return createSubdomainData(gridData);
    }

    this->gridData = gridData;

    this->setupBuild(domainPartitioner);
    auto localGridDataBuilder = boost::make_shared<LocalGridDataBuilder>(this);
    this->globalIndices = this->buildGlobalIndices(0);
    this->localGridData = boost::make_shared<GridData>(localGridDataBuilder->buildLocalGridData(0));
    this->verticesOfWells = this->buildVerticesOfWells();

    this->sendLocalGridDatas(localGridDataBuilder);
    this->sendGlobalIndices();
    return this->createSubdomainData();
}

SubdomainDataPtr DomainDivider::divideDomain() {
    this->receiveGridData();
    this->receiveGlobalIndices();
    return this->createSubdomainData();
}

void DomainDivider::setupBuild(DomainPartitioner domainPartitioner) {
    this->subdomains = domainPartitioner.partitionDomain(this->gridData, this->world.size());
    this->addLocalVertices();
    this->setSubdomainShiftsAndSizes();
    this->defineQuantities();
    this->findSubdomainsOfElements();
    this->findGhostVertices();
}

void DomainDivider::defineQuantities() {
    const auto& cs = this->gridData->connectivities;

    if (this->gridData->dimension == 2) {
        this->numberOfElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TRI_3 || c[0] == QUAD_4;});
        this->numberOfFacets = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == BAR_2;});
        this->numberOfWellElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == NODE;});
    }
    else  {
        this->numberOfElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TETRA_4 || c[0] == HEXA_8 || c[0] == PENTA_6 || c[0] == PYRA_5;});
        this->numberOfFacets = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TRI_3 || c[0] == QUAD_4;});
        this->numberOfWellElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == BAR_2;});
    }
}

void DomainDivider::addLocalVertices() {
    this->verticesLocalIndices.resize(this->gridData->coordinates.size());
    this->verticesOfSubdomains.resize(this->world.size());
    for (auto v = 0u; v < this->gridData->coordinates.size(); ++v) {
        std::pair<int, int> vertexLocalHandle{this->subdomains[v], this->verticesOfSubdomains[this->subdomains[v]].size()};
        this->verticesLocalIndices[v].push_back(vertexLocalHandle);
        this->verticesOfSubdomains[vertexLocalHandle.first].push_back(v);
    }
}

void DomainDivider::setSubdomainShiftsAndSizes() {
    this->subdomainShifts.resize(this->world.size() + 1, 0);
    this->subdomainSizes.resize(this->world.size(), 0);
    for (int rank = 1; rank < this->world.size() + 1; ++rank) {
        this->subdomainShifts[rank] = this->subdomainShifts[rank - 1] + this->verticesOfSubdomains[rank - 1].size();
        this->subdomainSizes[rank - 1] = this->subdomainShifts[rank] - this->subdomainShifts[rank - 1];
    }
}

void DomainDivider::findSubdomainsOfElements() {
    this->subdomainsOfElements.resize(this->numberOfElements + this->numberOfFacets + this->numberOfWellElements);
    this->elementsLocalIndices.resize(this->numberOfElements + this->numberOfFacets + this->numberOfWellElements, std::vector<int>(this->world.size(), -1));
    this->elementsOfSubdomains.resize(this->world.size());

    for (auto section : this->gridData->sections)
        this->addSubdomainsOfElements(section.begin, section.end);
}

void DomainDivider::addSubdomainsOfElements(int begin, int end) {
    for (auto position = this->gridData->connectivities.cbegin() + begin; position != this->gridData->connectivities.cbegin() + end; ++position) {
        int originalIndex = position->back();
        for (auto vertex = position->cbegin() + 1; vertex != position->cend() - 1; ++vertex) {
            int subdomain = this->subdomains[*vertex];
            if (this->subdomainsOfElements[originalIndex].insert(subdomain).second) {
                this->elementsLocalIndices[originalIndex][subdomain] = this->elementsOfSubdomains[subdomain].size();
                this->elementsOfSubdomains[subdomain].emplace_back(originalIndex);
            }
        }
    }
}

void DomainDivider::findGhostVertices() {
    for (auto section : this->gridData->sections) {
        if (section.dimension == this->gridData->dimension) {
            for (auto position = this->gridData->connectivities.cbegin() + section.begin; position != this->gridData->connectivities.cbegin() + section.end; ++position) {
                for (auto vertex = position->cbegin() + 1; vertex != position->cend() - 1; ++vertex) {
                    for (int subdomain : this->subdomainsOfElements[position->back()]) {
                        if (this->subdomains[*vertex] != subdomain) {
                            if (std::none_of(this->verticesLocalIndices[*vertex].cbegin(), this->verticesLocalIndices[*vertex].cend(), [=](auto s){return s.first == subdomain;})) {
                                std::pair<int, int> localHandle{subdomain, this->verticesOfSubdomains[subdomain].size()};
                                this->verticesLocalIndices[*vertex].emplace_back(localHandle);
                                this->verticesOfSubdomains[subdomain].emplace_back(*vertex);
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<int> DomainDivider::buildGlobalIndices(int rank) {
    std::vector<int> globalIndices;
    globalIndices.reserve(this->verticesOfSubdomains[rank].size());

    for (int vertex = 0; vertex < this->subdomainSizes[rank]; ++vertex) {
        globalIndices.emplace_back(vertex + this->subdomainShifts[rank]);
    }

    for (auto vertex = this->verticesOfSubdomains[rank].cbegin() + this->subdomainSizes[rank]; vertex < this->verticesOfSubdomains[rank].cend(); ++vertex) {
        auto indices = this->verticesLocalIndices[*vertex];
        auto index(std::find_if(indices.cbegin(), indices.cend(), [=](const auto& i){return i.first == this->subdomains[*vertex];}));
        globalIndices.emplace_back(index->second + this->subdomainShifts[this->subdomains[*vertex]]);
    }

    return globalIndices;
}

void DomainDivider::sendLocalGridDatas(boost::shared_ptr<LocalGridDataBuilder> localGridDataBuilder) {
    for (int rank = 1; rank < this->world.size(); ++rank)
        this->world.send(rank, rank, localGridDataBuilder->buildLocalGridData(rank));
}

void DomainDivider::receiveGridData() {
    this->localGridData = boost::make_shared<GridData>();
    this->world.recv(0, this->world.rank(), *this->localGridData);
}

void DomainDivider::sendGlobalIndices() {
    for (int rank = 1; rank < this->world.size(); ++rank)
        this->world.send(rank, rank, this->buildGlobalIndices(rank));
}

void DomainDivider::receiveGlobalIndices() {
    this->world.recv(0, this->world.rank(), this->globalIndices);
}

SubdomainDataPtr DomainDivider::createSubdomainData() {
    SubdomainDataPtr subdomainData{boost::make_shared<SubdomainData>(this->localGridData, this->globalIndices)};
    subdomainData->verticesOfSubdomains = this->verticesOfSubdomains;
    subdomainData->elementsOfSubdomains = this->elementsOfSubdomains;
    subdomainData->verticesOfWells = this->verticesOfWells;
    subdomainData->subdomainSizes = this->subdomainSizes;
    return subdomainData;
}

SubdomainDataPtr DomainDivider::createSubdomainData(GridDataPtr gridData) {
    SubdomainDataPtr subdomainData{boost::make_shared<SubdomainData>(gridData, std::vector<int>{})};
    subdomainData->subdomainSizes = std::vector<int>{0, gridData->numberOfLocalVertices};
    subdomainData->verticesOfWells = this->verticesOfWells;
    return subdomainData;
}

boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> DomainDivider::buildVerticesOfWells() {
    boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> verticesOfWells;
    for (auto section : this->gridData->sections) {
        if (this->gridData->dimension == 3 && section.dimension == 1) {
            verticesOfWells[section.name].reserve(section.vertices.size());
            for (auto vertex : section.vertices) {
                auto indices = this->verticesLocalIndices[vertex];
                auto local = std::find_if(indices.cbegin(), indices.cend(), [=](const auto& i){return i.first == this->subdomains[vertex];});
                int index = local->second + this->subdomainShifts[this->subdomains[vertex]];
                verticesOfWells[section.name].emplace_back(std::make_pair(index, this->gridData->coordinates[vertex]));
            }
        }
    }
    return verticesOfWells;
}

boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> DomainDivider::buildVerticesOfWells(GridDataPtr gridData) {
    boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> verticesOfWells;
    for (auto section : gridData->sections) {
        if (gridData->dimension == 3 && section.dimension == 1) {
            verticesOfWells[section.name].reserve(section.vertices.size());
            for (auto vertex : section.vertices) {
                verticesOfWells[section.name].emplace_back(std::make_pair(vertex, gridData->coordinates[vertex]));
            }
        }
    }
    return verticesOfWells;
}
