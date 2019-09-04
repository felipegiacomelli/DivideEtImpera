#include "DivideEtImpera/DomainDivider/SubdomainData.hpp"

SubdomainData::SubdomainData(GridDataPtr localGridData, std::vector<int> globalIndices) : localGridData(localGridData), globalIndices(globalIndices) {
    if (this->globalIndices.size() > 0u) {
        this->ghostGlobalIndices = std::vector<int>{this->globalIndices.cbegin() + this->localGridData->numberOfLocalVertices, this->globalIndices.cend()};
        this->ownershipRange = std::make_pair(this->globalIndices[0], this->globalIndices[this->localGridData->numberOfLocalVertices - 1]);
    }
    else {
        this->ownershipRange = std::make_pair(0, this->localGridData->numberOfLocalVertices);
    }
}
