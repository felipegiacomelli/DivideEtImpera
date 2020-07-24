#ifndef __DOMAIN_DIVIDER_DOMAIN_DIVIDER_HPP__
#define __DOMAIN_DIVIDER_DOMAIN_DIVIDER_HPP__

#include <list>
#include <MSHtoCGNS/Utilities/Vector.hpp>

#include "DivideEtImpera/BoostInterface/Serialization.hpp"
#include "DivideEtImpera/BoostInterface/Mpi.hpp"
#include "DivideEtImpera/MetisInterface/DomainPartitioner.hpp"
#include "DivideEtImpera/DomainDivider/SubdomainData.hpp"

class LocalGridDataBuilder;

class DomainDivider {
    friend class LocalGridDataBuilder;

    public:
        DomainDivider() = default;

        SubdomainDataPtr divideDomain(GridDataPtr gridData, DomainPartitioner domainPartitioner = DomainPartitioner{});
        SubdomainDataPtr divideDomain();

        virtual ~DomainDivider() = default;

    private:
        void setupBuild(DomainPartitioner domainPartitioner);
        void defineQuantities();
        void addLocalVertices();
        void setSubdomainShiftsAndSizes();

        void findSubdomainsOfElements();
        void addSubdomainsOfElements(int begin, int end);

        void findGhostVertices();

        std::vector<int> buildGlobalIndices(int rank);

        void sendLocalGridDatas(boost::shared_ptr<LocalGridDataBuilder> localGridDataBuilder);
        void receiveGridData();

        void sendGlobalIndices();
        void receiveGlobalIndices();

        SubdomainDataPtr createSubdomainData(GridDataPtr gridData);
        SubdomainDataPtr createSubdomainData();

        boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> buildVerticesOfWells();
        boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> buildVerticesOfWells(GridDataPtr gridData);

        GridDataPtr gridData;

        boost::mpi::communicator world;
        int numberOfElements, numberOfFacets, numberOfWellElements;

        std::vector<int> subdomains;
        std::vector<std::list<std::pair<int, int>>> verticesLocalIndices;
        std::vector<std::vector<int>> verticesOfSubdomains;

        std::vector<std::vector<int>> globalConnectivities;

        std::vector<std::set<int>> subdomainsOfElements;
        std::vector<std::vector<int>> elementsLocalIndices;
        std::vector<std::vector<int>> elementsOfSubdomains;

        std::vector<int> subdomainShifts;
        std::vector<int> subdomainSizes;

        GridDataPtr localGridData;
        std::vector<int> globalIndices;

        boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> verticesOfWells;
};

#endif
