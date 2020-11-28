#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/MetisInterface/DomainPartitioner.hpp"

#include <MSHtoCGNS/CgnsInterface/CgnsCreator.hpp>

struct DomainPartitionerFixtureI : public GridDataFixture {
    DomainPartitionerFixtureI() : GridDataFixture("15v_8e.cgns") {}
};

FixtureTestSuite(DomainPartitionerSuiteI, DomainPartitionerFixtureI)

TestCase(DomainPartitionerCase) {
    if (this->world.rank() == 0) {
        std::vector<int> subdomains;
        DomainPartitioner domainPartitioner;

        subdomains = domainPartitioner.partitionDomain(gridData, 2);

        std::vector<int> xadj{0, 2, 5, 8, 11, 13, 16, 20, 24, 28, 31, 33, 36, 39, 42, 44};
        std::vector<int> adjncy{1, 5, 0, 2, 6, 1, 3, 7, 2, 4, 8, 3, 9, 0, 6, 10, 1, 5, 7, 11, 2, 6, 8, 12, 3, 7, 9, 13, 4, 8, 14, 5, 11, 6, 10, 12, 7, 11, 13, 8, 12, 14, 9, 13};

        auto domainPartitionerXadj = domainPartitioner.getXadj();
        auto domainPartitionerAdjncy = domainPartitioner.getAdjncy();

        checkEqualCollections(domainPartitionerXadj.cbegin(), domainPartitionerXadj.cend(), xadj.cbegin(), xadj.cend());
        checkEqualCollections(domainPartitionerAdjncy.cbegin(), domainPartitionerAdjncy.cend(), adjncy.cbegin(), adjncy.cend());
    }
}

TestSuiteEnd()
