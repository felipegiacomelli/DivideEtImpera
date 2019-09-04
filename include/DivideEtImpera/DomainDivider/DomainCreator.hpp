#ifndef __DOMAIN_DIVIDER_DOMAIN_CREATOR_HPP__
#define __DOMAIN_DIVIDER_DOMAIN_CREATOR_HPP__

#include <MSHtoCGNS/CgnsInterface/CgnsCreator.hpp>

#include "DivideEtImpera/BoostInterface/Mpi.hpp"
#include "DivideEtImpera/DomainDivider/SubdomainData.hpp"

class DomainCreator {
    public:
        DomainCreator(GridDataPtr globalGridData, SubdomainDataPtr subdomainData, std::string outputPath);

        std::string getFileName() const;

        ~DomainCreator() = default;

    private:
        void setOldToNew();
        void reorderCoordinates();
        void rectifyConnectivities();

        template<typename T> void rectifyConnectivity(std::vector<T>& connectivities) {
            for (auto& connectivity : connectivities)
                for (auto index = connectivity.begin(); index != connectivity.end() - 1; ++index)
                    *index = this->oldToNew[*index];
        }

        void createOutputFile();

        boost::shared_ptr<GridData> gridData;
        SubdomainDataPtr subdomainData;
        std::string outputPath;

        boost::mpi::communicator world;
        std::vector<int> sizes;
        std::unordered_map<int, int> oldToNew;
        std::string fileName;
};

#endif
