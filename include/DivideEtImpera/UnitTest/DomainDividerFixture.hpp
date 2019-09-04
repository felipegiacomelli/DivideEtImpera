#ifndef __UNIT_TEST_DOMAIN_DIVIDER_FIXTURE_HPP__
#define __UNIT_TEST_DOMAIN_DIVIDER_FIXTURE_HPP__

#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"

struct DomainDividerFixture : public GridDataFixture {
    DomainDividerFixture(std::string path) : GridDataFixture(path) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            this->subdomainData = domainDivider.divideDomain(this->gridData);
        }
        else {
            DomainDivider domainDivider;
            this->subdomainData = domainDivider.divideDomain();
        }
    }

    SubdomainDataPtr subdomainData;
};

#endif
