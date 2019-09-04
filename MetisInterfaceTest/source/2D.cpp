#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"

struct PartitionerFixtureI : public GridDataFixture {
    PartitionerFixtureI() : GridDataFixture("9v_4e_2d.cgns") {}
};

FixtureTestSuite(PartitionerSuiteI, PartitionerFixtureI)

TestCase(PartitionerCase) {
    if (this->world.rank() == 0) {
        std::vector<int> subdomains;
        DomainPartitioner domainPartitioner;

        subdomains = domainPartitioner.partitionDomain(gridData, 2);
        checkEqual(subdomains[0], 1);
        checkEqual(subdomains[1], 1);
        checkEqual(subdomains[2], 1);
        checkEqual(subdomains[3], 0);
        checkEqual(subdomains[4], 1);
        checkEqual(subdomains[5], 0);
        checkEqual(subdomains[6], 0);
        checkEqual(subdomains[7], 0);
        checkEqual(subdomains[8], 0);

        subdomains = domainPartitioner.partitionDomain(gridData, 3);
        checkEqual(subdomains[0], 0);
        checkEqual(subdomains[1], 1);
        checkEqual(subdomains[2], 1);
        checkEqual(subdomains[3], 0);
        checkEqual(subdomains[4], 2);
        checkEqual(subdomains[5], 1);
        checkEqual(subdomains[6], 0);
        checkEqual(subdomains[7], 2);
        checkEqual(subdomains[8], 2);
    }
}

TestSuiteEnd()

struct PartitionerFixtureII : public GridDataFixture {
    PartitionerFixtureII() : GridDataFixture("6v_4e_2d.cgns") {}
};

FixtureTestSuite(PartitionerSuiteII, PartitionerFixtureII)

TestCase(PartitionerCase) {
    if (this->world.rank() == 0) {
        std::vector<int> subdomains;
        DomainPartitioner domainPartitioner;

        subdomains = domainPartitioner.partitionDomain(gridData, 2);
        checkEqual(subdomains[0], 0);
        checkEqual(subdomains[1], 1);
        checkEqual(subdomains[2], 0);
        checkEqual(subdomains[3], 0);
        checkEqual(subdomains[4], 1);
        checkEqual(subdomains[5], 1);
    }
}

TestSuiteEnd()
