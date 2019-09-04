#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include "DivideEtImpera/DomainDivider/LocalGridDataBuilder.hpp"
#include <cgnslib.h>

LocalGridDataBuilder::LocalGridDataBuilder(DomainDivider* domainDivider) : domainDivider(domainDivider) {}

GridData LocalGridDataBuilder::buildLocalGridData(int rank) {
    GridData localGridData;
    localGridData.dimension = this->domainDivider->gridData->dimension;
    localGridData.numberOfLocalVertices = this->domainDivider->subdomainSizes[rank];

    this->addVertices(rank, localGridData);
    this->setOriginalToLocal(rank);
    this->addElements(rank, localGridData);
    this->buildLocalConnectivities(rank, localGridData);
    this->addEntities(rank, this->domainDivider->gridData->regions, localGridData.regions);
    this->addEntities(rank, this->domainDivider->gridData->boundaries, localGridData.boundaries);
    this->addEntities(rank, this->domainDivider->gridData->wells, localGridData.wells);
    this->findVertices(localGridData.regions);
    this->findVertices(localGridData.boundaries);
    this->findVertices(localGridData.wells);

    return localGridData;
}

void LocalGridDataBuilder::addVertices(int rank, GridData& localGridData) {
    localGridData.coordinates.reserve(this->domainDivider->verticesOfSubdomains[rank].size());
    for (auto vertex : this->domainDivider->verticesOfSubdomains[rank]) {
        localGridData.coordinates.emplace_back(this->domainDivider->gridData->coordinates[vertex]);
    }
}

void LocalGridDataBuilder::setOriginalToLocal(int rank) {
    this->originalToLocal.clear();
    this->originalToLocal.reserve(this->domainDivider->verticesOfSubdomains[rank].size());
    for (auto vertex : this->domainDivider->verticesOfSubdomains[rank]) {
        auto indices = this->domainDivider->verticesLocalIndices[vertex];
        auto local = std::find_if(indices.cbegin(), indices.cend(), [=](const auto& i){return i.first == rank;});
        this->originalToLocal[vertex] = local->second;
    }
}

void LocalGridDataBuilder::addElements(int rank, GridData& localGridData) {
    for (auto element : this->domainDivider->elementsOfSubdomains[rank]) {
        auto connectivity = this->domainDivider->globalConnectivities[element];
        switch (connectivity.front()) {
            case TETRA_4:
                this->addElement(rank, connectivity, localGridData.tetrahedrons);
                break;
            case HEXA_8:
                this->addElement(rank, connectivity, localGridData.hexahedrons);
                break;
            case PENTA_6:
                this->addElement(rank, connectivity, localGridData.prisms);
                break;
            case PYRA_5:
                this->addElement(rank, connectivity, localGridData.pyramids);
                break;
            case TRI_3:
                this->addElement(rank, connectivity, localGridData.triangles);
                break;
            case QUAD_4:
                this->addElement(rank, connectivity, localGridData.quadrangles);
                break;
            case BAR_2:
                this->addElement(rank, connectivity, localGridData.lines);
                break;
        }
    }
}

void LocalGridDataBuilder::buildLocalConnectivities(int rank, const GridData& localGridData) {
    this->localConnectivities.clear();
    this->localConnectivities.reserve(this->domainDivider->elementsOfSubdomains[rank].size());

    append(localGridData.tetrahedrons, this->localConnectivities);
    append(localGridData.hexahedrons, this->localConnectivities);
    append(localGridData.prisms, this->localConnectivities);
    append(localGridData.pyramids, this->localConnectivities);
    append(localGridData.triangles, this->localConnectivities);
    append(localGridData.quadrangles, this->localConnectivities);
    append(localGridData.lines, this->localConnectivities);

    std::sort(this->localConnectivities.begin(), this->localConnectivities.end(), [](const auto& a, const auto& b){return a.back() < b.back();});
}

void LocalGridDataBuilder::addEntities(int rank, const std::vector<EntityData>& originals, std::vector<EntityData>& locals) {
    for (auto original : originals) {
        bool firstElementToAdd = true;
        for (int element = original.begin; element < original.end; ++element) {
            if (this->domainDivider->elementsLocalIndices[element][rank] != -1) {
                if (firstElementToAdd) {
                    firstElementToAdd = false;
                    locals.emplace_back(EntityData{original.name, this->domainDivider->elementsLocalIndices[element][rank], -1});
                }
                locals.back().end = this->domainDivider->elementsLocalIndices[element][rank] + 1;
            }
        }
    }
}

void LocalGridDataBuilder::findVertices(std::vector<EntityData>& entities) {
    for (auto& entity : entities) {
        std::set<int> vertices;
        for (auto position = this->localConnectivities.cbegin() + entity.begin; position != this->localConnectivities.cbegin() + entity.end; ++position) {
            vertices.insert(position->cbegin(), position->cend() - 1);
        }
        entity.vertices = std::vector<int>{vertices.begin(), vertices.end()};
    }
}
