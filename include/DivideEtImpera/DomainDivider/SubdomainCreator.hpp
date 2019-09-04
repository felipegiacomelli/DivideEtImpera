#ifndef __DOMAIN_DIVIDER_SUBDOMAIN_CREATOR_HPP__
#define __DOMAIN_DIVIDER_SUBDOMAIN_CREATOR_HPP__

#include <MSHtoCGNS/CgnsInterface/CgnsCreator.hpp>

#include "DivideEtImpera/BoostInterface/Mpi.hpp"
#include "DivideEtImpera/DomainDivider/SubdomainData.hpp"

class SubdomainCreator {
    public:
        SubdomainCreator(GridDataPtr localGridData, std::string outputPath, bool removeGhosts = true);

        std::string getFileName() const;

        ~SubdomainCreator() = default;

    private:
        void removeGhostVertices();
        void removeGhostElements();

        template<typename T> void removeGhostElements(std::vector<T>& connectivities) {
            for (auto connectivity = connectivities.begin(); connectivity != connectivities.end();)
                if (std::any_of(connectivity->begin(), connectivity->end() - 1, [=](auto v){return v >= this->gridData->numberOfLocalVertices;}))
                    connectivity = connectivities.erase(connectivity);
                else
                    ++connectivity;
        }

        void defineQuantities();
        void createRegion();
        void createBoundary();
        void createWell();

        void createOutputFile();

        boost::shared_ptr<GridData> gridData;
        boost::filesystem::path output;

        boost::mpi::communicator world;
        int numberOfElements, numberOfFacets, numberOfWellElements;
        int begin, end;
        std::string fileName;
};

#endif
