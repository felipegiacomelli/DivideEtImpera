#include "DivideEtImpera/DomainDivider/DomainCreator.hpp"

DomainCreator::DomainCreator(GridDataPtr globalGridData, SubdomainDataPtr subdomainData, std::string outputPath) :
    gridData(boost::make_shared<GridData>(*globalGridData)), subdomainData(subdomainData), outputPath(outputPath) {
    if (this->world.size() > 1) {
        this->sizes = this->subdomainData->subdomainSizes;
        this->setOldToNew();
        this->reorderCoordinates();
        this->rectifyConnectivities();
    }
    this->createOutputFile();
}

void DomainCreator::setOldToNew() {
    this->oldToNew.reserve(this->gridData->coordinates.size());
    int counter = 0;
    for (int rank = 0; rank < world.size(); ++rank) {
        for (int vertex = 0; vertex < this->sizes[rank]; ++vertex) {
            this->oldToNew[this->subdomainData->verticesOfSubdomains[rank][vertex]] = counter++;
        }
    }
}

void DomainCreator::reorderCoordinates() {
    std::vector<std::array<double, 3>> coordinates;
    coordinates.reserve(this->gridData->coordinates.size());
    for (int rank = 0; rank < this->world.size(); ++rank) {
        for (int vertex = 0; vertex < this->sizes[rank]; ++vertex) {
            coordinates.emplace_back(this->gridData->coordinates[this->subdomainData->verticesOfSubdomains[rank][vertex]]);
        }
    }
    this->gridData->coordinates = coordinates;
}

void DomainCreator::rectifyConnectivities() {
    for (auto& connectivity : this->gridData->connectivities)
        for (auto index = connectivity.begin() + 1; index != connectivity.end() - 1; ++index)
            *index = this->oldToNew[*index];
}

void DomainCreator::createOutputFile() {
    CgnsCreator cgnsCreator(this->gridData, this->outputPath);
    this->fileName = cgnsCreator.getFileName();
}

std::string DomainCreator::getFileName() const {
    return this->fileName;
}
