#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include <cgnslib.h>

#define TOLERANCE 1.0e-12

struct DomainDividerWell3DFixture : public GridDataFixture {
    DomainDividerWell3DFixture() : GridDataFixture("150v_80e.cgns") {}
};

FixtureTestSuite(DomainDividerWell3DSuite, DomainDividerWell3DFixture)

TestCase(VerticesOfSubdomainsCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);

            {
                auto well = subdomainData->localGridData->sections[6];
                std::vector<int> expected{0, 23, 24, 81};

                checkEqual(well.name, "INJECTOR");
                checkEqualCollections(well.vertices.cbegin(), well.vertices.cend(), expected.cbegin(), expected.cend());

                checkEqual(subdomainData->globalIndices[ 0],  0);
                checkEqual(subdomainData->globalIndices[23], 23);
                checkEqual(subdomainData->globalIndices[24], 24);
                checkEqual(subdomainData->globalIndices[81], 86);
            }

            {
                auto well = subdomainData->localGridData->sections[7];
                std::vector<int> expected{8, 33, 34, 93};

                checkEqual(well.name, "PRODUCER");
                checkEqualCollections(well.vertices.cbegin(), well.vertices.cend(), expected.cbegin(), expected.cend());

                checkEqual(subdomainData->globalIndices[ 8],   8);
                checkEqual(subdomainData->globalIndices[33],  33);
                checkEqual(subdomainData->globalIndices[34],  34);
                checkEqual(subdomainData->globalIndices[93], 103);
            }

            checkEqual(subdomainData->verticesOfWells.size(), 2u);

            {
                auto verticesOfWell = subdomainData->verticesOfWells["INJECTOR"];
                std::sort(verticesOfWell.begin(), verticesOfWell.end(), [](auto a, auto b){return a.second[2] < b.second[2];});
                checkEqual(verticesOfWell[0].first,  0);
                checkEqual(verticesOfWell[1].first, 23);
                checkEqual(verticesOfWell[2].first, 24);
                checkEqual(verticesOfWell[3].first, 86);
                checkEqual(verticesOfWell[4].first, 87);
                checkEqual(verticesOfWell[5].first, 76);
            }

            {
                auto verticesOfWell = subdomainData->verticesOfWells["PRODUCER"];
                std::sort(verticesOfWell.begin(), verticesOfWell.end(), [](auto a, auto b){return a.second[2] < b.second[2];});
                checkEqual(verticesOfWell[0].first,   8);
                checkEqual(verticesOfWell[1].first,  33);
                checkEqual(verticesOfWell[2].first,  34);
                checkEqual(verticesOfWell[3].first, 103);
                checkEqual(verticesOfWell[4].first, 104);
                checkEqual(verticesOfWell[5].first,  81);
            }
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            std::vector<std::vector<int>> verticesOfSubdomains = subdomainData->verticesOfSubdomains;

            {
                auto well = subdomainData->localGridData->sections[6];
                std::vector<int> expected{1, 11, 12, 81};

                checkEqual(well.name, "INJECTOR");
                checkEqualCollections(well.vertices.cbegin(), well.vertices.cend(), expected.cbegin(), expected.cend());

                checkEqual(subdomainData->globalIndices[ 1], 76);
                checkEqual(subdomainData->globalIndices[11], 86);
                checkEqual(subdomainData->globalIndices[12], 87);
                checkEqual(subdomainData->globalIndices[81], 24);
            }

            {
                auto well = subdomainData->localGridData->sections[7];
                std::vector<int> expected{6, 28, 29, 93};

                checkEqual(well.name, "PRODUCER");
                checkEqualCollections(well.vertices.cbegin(), well.vertices.cend(), expected.cbegin(), expected.cend());

                checkEqual(subdomainData->globalIndices[ 6],  81);
                checkEqual(subdomainData->globalIndices[28], 103);
                checkEqual(subdomainData->globalIndices[29], 104);
                checkEqual(subdomainData->globalIndices[93],  34);
            }
        }
    }
}

TestSuiteEnd()
