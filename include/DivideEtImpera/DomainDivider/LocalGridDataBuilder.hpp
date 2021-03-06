#ifndef __GRID_DATA_DIVIDER_LOCAL_GRID_DATA_BUILDER_HPP__
#define __GRID_DATA_DIVIDER_LOCAL_GRID_DATA_BUILDER_HPP__

#include <set>

#include <boost/unordered_map.hpp>

#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"

class DomainDivider;

class LocalGridDataBuilder {
    public:
        LocalGridDataBuilder() = default;
        LocalGridDataBuilder(DomainDivider* domainDivider);

        GridData buildLocalGridData(int rank);

        virtual ~LocalGridDataBuilder() = default;

    private:
        void addVertices(int rank, GridData& localGridData);
        void setOriginalToLocal(int rank);
        void addElements(int rank, GridData& localGridData);
        void addEntities(int rank, const std::vector<SectionData>& originals, std::vector<SectionData>& locals);
        void findVertices(std::vector<SectionData>& sections, GridData& gridData);

        DomainDivider* domainDivider;

        boost::unordered_map<int, int> originalToLocal;
        std::vector<std::vector<int>> localConnectivities;
};

#endif
