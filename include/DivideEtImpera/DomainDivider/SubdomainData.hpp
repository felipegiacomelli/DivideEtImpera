#ifndef __DOMAIN_DIVIDER_SUBDOMAIN_DATA_OUTPUT__
#define __DOMAIN_DIVIDER_SUBDOMAIN_DATA_OUTPUT__

#include <MSHtoCGNS/GridData/GridData.hpp>

struct SubdomainData {
    SubdomainData() = default;
    SubdomainData(GridDataPtr localGridData, std::vector<int> globalIndices);

    GridDataPtr localGridData;
    std::vector<int> globalIndices;

    std::vector<int> ghostGlobalIndices;
    std::pair<int, int> ownershipRange;

    std::vector<std::vector<int>> verticesOfSubdomains;
    std::vector<std::vector<int>> elementsOfSubdomains;
    std::vector<int> subdomainSizes;
};

using SubdomainDataPtr = boost::shared_ptr<SubdomainData>;

#endif
