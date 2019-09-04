#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include "DivideEtImpera/DomainDivider/LocalGridDataBuilder.hpp"
#include <cgnslib.h>

SubdomainDataPtr DomainDivider::divideDomain(GridDataPtr gridData, DomainPartitioner domainPartitioner) {
    if (this->world.size() == 1) {
        return createSubdomainData(gridData);
    }

    this->gridData = gridData;

    this->setupBuild(domainPartitioner);
    auto localGridDataBuilder = boost::make_shared<LocalGridDataBuilder>(this);
    this->globalIndices = this->buildGlobalIndices(0);
    this->localGridData = boost::make_shared<GridData>(localGridDataBuilder->buildLocalGridData(0));

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
    this->buildGlobalConnectivities();
    this->findSubdomainsOfElements();
    this->findGhostVertices();
}

void DomainDivider::defineQuantities() {
    if (this->gridData->dimension == 2) {
        this->numberOfElements = this->gridData->triangles.size() + this->gridData->quadrangles.size();
        this->numberOfFacets = this->gridData->lines.size();
        this->numberOfWellElements = 0;
    }
    else  {
        this->numberOfElements = this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size();
        this->numberOfFacets = this->gridData->triangles.size() + this->gridData->quadrangles.size();
        this->numberOfWellElements = this->gridData->lines.size();
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

void DomainDivider::buildGlobalConnectivities() {
    this->globalConnectivities.reserve(this->numberOfElements + this->numberOfFacets + this->numberOfWellElements);

    append(TETRA_4, this->gridData->tetrahedrons, this->globalConnectivities);
    append(HEXA_8, this->gridData->hexahedrons, this->globalConnectivities);
    append(PENTA_6, this->gridData->prisms, this->globalConnectivities);
    append(PYRA_5, this->gridData->pyramids, this->globalConnectivities);
    append(TRI_3, this->gridData->triangles, this->globalConnectivities);
    append(QUAD_4, this->gridData->quadrangles, this->globalConnectivities);
    append(BAR_2, this->gridData->lines, this->globalConnectivities);

    std::sort(this->globalConnectivities.begin(), this->globalConnectivities.end(), [](const auto& a, const auto& b){return a.back() < b.back();});
}

void DomainDivider::findSubdomainsOfElements() {
    this->subdomainsOfElements.resize(this->numberOfElements + this->numberOfFacets + this->numberOfWellElements);
    this->elementsLocalIndices.resize(this->numberOfElements + this->numberOfFacets + this->numberOfWellElements, std::vector<int>(this->world.size(), -1));
    this->elementsOfSubdomains.resize(this->world.size());

    for (auto region : this->gridData->regions)
        this->addSubdomainsOfElements(region.begin, region.end);

    for (auto boundary : this->gridData->boundaries)
        this->addSubdomainsOfElements(boundary.begin, boundary.end);

    for (auto well : this->gridData->wells)
        this->addSubdomainsOfElements(well.begin, well.end);
}

void DomainDivider::addSubdomainsOfElements(int begin, int end) {
    for (auto position = this->globalConnectivities.cbegin() + begin; position != this->globalConnectivities.cbegin() + end; ++position) {
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
    for (auto region : this->gridData->regions) {
        for (auto position = this->globalConnectivities.cbegin() + region.begin; position != this->globalConnectivities.cbegin() + region.end; ++position) {
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
    subdomainData->subdomainSizes = this->subdomainSizes;
    return subdomainData;
}

SubdomainDataPtr DomainDivider::createSubdomainData(GridDataPtr gridData) {
    SubdomainDataPtr subdomainData{boost::make_shared<SubdomainData>(gridData, std::vector<int>{})};
    subdomainData->subdomainSizes = std::vector<int>{0, gridData->numberOfLocalVertices};
    return subdomainData;
}
