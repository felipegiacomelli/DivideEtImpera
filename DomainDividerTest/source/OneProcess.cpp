#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"

struct DomainDividerOneProcessFixture : public GridDataFixture {
    DomainDividerOneProcessFixture() : GridDataFixture("6v_4e_2d.cgns") {}
};

FixtureTestSuite(DomainDividerOneProcessSuite, DomainDividerOneProcessFixture)

TestCase(VerticesOfSubdomainsCase) {
    if (this->world.size() == 1) {
        DomainDivider domainDivider;
        SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
        checkEqual(subdomainData->ghostGlobalIndices.size(), 0u);
        checkEqual(subdomainData->ownershipRange.first, 0);
        checkEqual(subdomainData->ownershipRange.second, 5);
        checkEqual(subdomainData->globalIndices.size(), 0u);
    }
}

TestSuiteEnd()
