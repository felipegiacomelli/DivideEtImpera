#ifndef __DOMAIN_DIVIDER_SUBDOMAIN_DATA_OUTPUT__
#define __DOMAIN_DIVIDER_SUBDOMAIN_DATA_OUTPUT__

#include <boost/unordered_map.hpp>
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
    boost::unordered_map<std::string, std::vector<std::pair<int, std::array<double, 3>>>> verticesOfWells;
    std::vector<int> subdomainSizes;
};

using SubdomainDataPtr = boost::shared_ptr<SubdomainData>;

#endif
