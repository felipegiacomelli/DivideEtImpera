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
        void createOutputFile();

        boost::shared_ptr<GridData> gridData;
        SubdomainDataPtr subdomainData;
        std::string outputPath;

        boost::mpi::communicator world;
        std::vector<int> sizes;
        boost::unordered_map<int, int> oldToNew;
        std::string fileName;
};

#endif
