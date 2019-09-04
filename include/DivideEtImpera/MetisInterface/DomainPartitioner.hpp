#ifndef __METIS_INTERFACE_DOMAIN_PARTITIONER_HPP__
#define __METIS_INTERFACE_DOMAIN_PARTITIONER_HPP__

#include <numeric>
#include <MSHtoCGNS/GridData/GridData.hpp>

#include "DivideEtImpera/MetisInterface/GraphBuilder.hpp"

class DomainPartitioner {
    public:
        DomainPartitioner() = default;

        std::vector<int> partitionDomain(GridDataPtr gridData, int numberOfPartitions);

        ~DomainPartitioner() = default;

    private:
        void buildMetisVectors(GridDataPtr gridData);

        std::vector<int> xadj;
        std::vector<int> adjncy;
        GraphBuilder graphBuilder;
};

#endif
