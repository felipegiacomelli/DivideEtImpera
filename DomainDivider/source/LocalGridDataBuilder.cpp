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
    this->addEntities(rank, this->domainDivider->gridData->entities, localGridData.entities);
    this->findVertices(localGridData.entities, localGridData);

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
    localGridData.connectivities.reserve(this->domainDivider->elementsOfSubdomains[rank].size());
    for (auto element : this->domainDivider->elementsOfSubdomains[rank]) {
        auto& connectivity = localGridData.connectivities.emplace_back(this->domainDivider->gridData->connectivities[element]);
        for (auto vertex = connectivity.begin() + 1; vertex != connectivity.end() - 1; ++vertex) {
            *vertex = this->originalToLocal[*vertex];
        }
        connectivity.back() = this->domainDivider->elementsLocalIndices[connectivity.back()][rank];
    }
    std::sort(localGridData.connectivities.begin(), localGridData.connectivities.end(), [](const auto& a, const auto& b){return a.back() < b.back();});
}

void LocalGridDataBuilder::addEntities(int rank, const std::vector<EntityData>& originals, std::vector<EntityData>& locals) {
    for (auto original : originals) {
        bool firstElementToAdd = true;
        for (int element = original.begin; element < original.end; ++element) {
            if (this->domainDivider->elementsLocalIndices[element][rank] != -1) {
                if (firstElementToAdd) {
                    firstElementToAdd = false;
                    locals.emplace_back(EntityData{original.name, original.dimension, this->domainDivider->elementsLocalIndices[element][rank], -1, std::vector<int>{}});
                }
                locals.back().end = this->domainDivider->elementsLocalIndices[element][rank] + 1;
            }
        }
    }
}

void LocalGridDataBuilder::findVertices(std::vector<EntityData>& entities, GridData& localGridData) {
    for (auto& entity : entities) {
        std::set<int> vertices;
        for (auto position = localGridData.connectivities.cbegin() + entity.begin; position != localGridData.connectivities.cbegin() + entity.end; ++position) {
            vertices.insert(position->cbegin() + 1, position->cend() - 1);
        }
        entity.vertices = std::vector<int>{vertices.begin(), vertices.end()};
    }
}
