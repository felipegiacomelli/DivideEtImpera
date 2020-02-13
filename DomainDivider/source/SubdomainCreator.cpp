#include "DivideEtImpera/DomainDivider/SubdomainCreator.hpp"
#include <cgnslib.h>

SubdomainCreator::SubdomainCreator(GridDataPtr localGridData, std::string outputPath, bool removeGhosts) : gridData(boost::make_shared<GridData>(*localGridData)), output(outputPath) {
    if (removeGhosts) {
        this->removeGhostVertices();
        this->removeGhostElements();
    }
    this->defineQuantities();
    this->gridData->sections.clear();
    this->createRegion();
    this->createBoundary();
    this->createWell();
    this->createOutputFile();
}

void SubdomainCreator::removeGhostVertices() {
    this->gridData->coordinates.erase(this->gridData->coordinates.begin() + this->gridData->numberOfLocalVertices, this->gridData->coordinates.end());
}

void SubdomainCreator::removeGhostElements() {
    for (auto connectivity = this->gridData->connectivities.begin(); connectivity != this->gridData->connectivities.end();)
        if (std::any_of(connectivity->begin() + 1, connectivity->end() - 1, [=](auto v){return v >= this->gridData->numberOfLocalVertices;}))
            connectivity = this->gridData->connectivities.erase(connectivity);
        else
            ++connectivity;
}

void SubdomainCreator::defineQuantities() {
    const auto& cs = this->gridData->connectivities;

    if (this->gridData->dimension == 2) {
        this->numberOfElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TRI_3 || c[0] == QUAD_4;});
        this->numberOfFacets = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == BAR_2;});
        this->numberOfWellElements = 0;
    }
    else if (this->gridData->dimension == 3) {
        this->numberOfElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TETRA_4 || c[0] == HEXA_8 || c[0] == PENTA_6 || c[0] == PYRA_5;});
        this->numberOfFacets = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == TRI_3 || c[0] == QUAD_4;});
        this->numberOfWellElements = std::count_if(cs.cbegin(), cs.cend(), [](const auto& c){return c[0] == BAR_2;});
    }
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - gridData dimension must be either 2 or 3");
}

void SubdomainCreator::createRegion() {
    this->begin = 0;
    this->end = this->numberOfElements;
    this->gridData->sections.emplace_back(SectionData{"REGION", this->gridData->dimension, this->begin, this->end, std::vector<int>{}});
}

void SubdomainCreator::createBoundary() {
    this->begin += this->numberOfElements;
    this->end += this->numberOfFacets;
    if (this->numberOfFacets > 0) {
        this->gridData->sections.emplace_back(SectionData{"BOUNDARY", this->gridData->dimension - 1, this->begin, this->end, std::vector<int>{}});
    }
}

void SubdomainCreator::createWell() {
    this->begin += this->numberOfFacets;
    this->end += this->numberOfWellElements;
    if (this->numberOfWellElements > 0) {
        this->gridData->sections.emplace_back(SectionData{"WELL", 1, this->begin, this->end, std::vector<int>{}});
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
