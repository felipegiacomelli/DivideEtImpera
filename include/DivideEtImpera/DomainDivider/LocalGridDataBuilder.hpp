#ifndef __GRID_DATA_DIVIDER_LOCAL_GRID_DATA_BUILDER_HPP__
#define __GRID_DATA_DIVIDER_LOCAL_GRID_DATA_BUILDER_HPP__

#include <unordered_map>
#include <set>

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
        void addEntities(int rank, const std::vector<EntityData>& originals, std::vector<EntityData>& locals);
        void findVertices(std::vector<EntityData>& entities, GridData& gridData);

        DomainDivider* domainDivider;

        std::unordered_map<int, int> originalToLocal;
        std::vector<std::vector<int>> localConnectivities;
};

#endif
