#include "DivideEtImpera/DomainDivider/SubdomainCreator.hpp"

SubdomainCreator::SubdomainCreator(GridDataPtr localGridData, std::string outputPath, bool removeGhosts) : gridData(boost::make_shared<GridData>(*localGridData)), output(outputPath) {
    if (removeGhosts) {
        this->removeGhostVertices();
        this->removeGhostElements();
    }
    this->defineQuantities();
    this->createRegion();
    this->createBoundary();
    this->createWell();
    this->createOutputFile();
}

void SubdomainCreator::removeGhostVertices() {
    this->gridData->coordinates.erase(this->gridData->coordinates.begin() + this->gridData->numberOfLocalVertices, this->gridData->coordinates.end());
}

void SubdomainCreator::removeGhostElements() {
    this->removeGhostElements(this->gridData->tetrahedrons);
    this->removeGhostElements(this->gridData->hexahedrons);
    this->removeGhostElements(this->gridData->prisms);
    this->removeGhostElements(this->gridData->pyramids);
    this->removeGhostElements(this->gridData->triangles);
    this->removeGhostElements(this->gridData->quadrangles);
    this->removeGhostElements(this->gridData->lines);
}

void SubdomainCreator::defineQuantities() {
    if (this->gridData->dimension == 2) {
        this->numberOfElements = this->gridData->triangles.size() + this->gridData->quadrangles.size();
        this->numberOfFacets = this->gridData->lines.size();
        this->numberOfWellElements = 0;
    }
    else if (this->gridData->dimension == 3) {
        this->numberOfElements = this->gridData->tetrahedrons.size() + this->gridData->hexahedrons.size() + this->gridData->prisms.size() + this->gridData->pyramids.size();
        this->numberOfFacets = this->gridData->triangles.size() + this->gridData->quadrangles.size();
        this->numberOfWellElements = this->gridData->lines.size();
    }
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be either 2 or 3");
}

void SubdomainCreator::createRegion() {
    this->begin = 0;
    this->end = this->numberOfElements;
    this->gridData->regions.clear();
    this->gridData->regions.emplace_back(EntityData{"REGION", this->begin, this->end});
}

void SubdomainCreator::createBoundary() {
    this->begin += this->numberOfElements;
    this->end += this->numberOfFacets;
    if (this->gridData->boundaries.size() > 0u) {
        this->gridData->boundaries.clear();
        this->gridData->boundaries.emplace_back(EntityData{"BOUNDARY", this->begin, this->end});
    }
}

void SubdomainCreator::createWell() {
    this->begin += this->numberOfFacets;
    this->end += this->numberOfWellElements;
    if (this->gridData->wells.size() > 0u) {
        this->gridData->wells.clear();
        this->gridData->wells.emplace_back(EntityData{"WELL", this->begin, this->end});
    }
}

void SubdomainCreator::createOutputFile() {
    if (this->output.extension() != std::string(".cgns"))
        this->output /= boost::filesystem::path(std::to_string(this->world.rank()) + ".cgns");

    CgnsCreator cgnsCreator(this->gridData, this->output.string());
    this->fileName = cgnsCreator.getFileName();
}

std::string SubdomainCreator::getFileName() const {
    return this->fileName;
}
