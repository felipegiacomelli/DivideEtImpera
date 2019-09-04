#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/MetisInterface/DomainPartitioner.hpp"

struct PartitionerFixtureIII : public GridDataFixture {
    PartitionerFixtureIII() : GridDataFixture("14v_24e_3d.cgns") {}
};

FixtureTestSuite(PartitionerSuiteIII, PartitionerFixtureIII)

TestCase(PartitionerCase) {
    if (this->world.rank() == 0) {
        std::vector<int> subdomains;
        DomainPartitioner domainPartitioner;

        subdomains = domainPartitioner.partitionDomain(gridData, 2);
        checkEqual(subdomains[ 0], 1);
        checkEqual(subdomains[ 1], 0);
        checkEqual(subdomains[ 2], 1);
        checkEqual(subdomains[ 3], 1);
        checkEqual(subdomains[ 4], 0);
        checkEqual(subdomains[ 5], 0);
        checkEqual(subdomains[ 6], 0);
        checkEqual(subdomains[ 7], 1);
        checkEqual(subdomains[ 8], 1);
        checkEqual(subdomains[ 9], 0);
        checkEqual(subdomains[10], 0);
        checkEqual(subdomains[11], 1);
        checkEqual(subdomains[12], 1);
        checkEqual(subdomains[13], 0);

        subdomains = domainPartitioner.partitionDomain(gridData, 3);
        checkEqual(subdomains[ 0], 2);
        checkEqual(subdomains[ 1], 1);
        checkEqual(subdomains[ 2], 0);
        checkEqual(subdomains[ 3], 1);
        checkEqual(subdomains[ 4], 2);
        checkEqual(subdomains[ 5], 1);
        checkEqual(subdomains[ 6], 1);
        checkEqual(subdomains[ 7], 0);
        checkEqual(subdomains[ 8], 0);
        checkEqual(subdomains[ 9], 0);
        checkEqual(subdomains[10], 1);
        checkEqual(subdomains[11], 2);
        checkEqual(subdomains[12], 2);
        checkEqual(subdomains[13], 2);
    }
}

TestSuiteEnd()
