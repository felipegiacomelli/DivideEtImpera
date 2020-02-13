#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include <cgnslib.h>

#define TOLERANCE 1.0e-12

struct DomainDivider2DFixture : public GridDataFixture {
    DomainDivider2DFixture() : GridDataFixture("6v_4e_2d.cgns") {}
};

FixtureTestSuite(DomainDivider2DSuite, DomainDivider2DFixture)

TestCase(VerticesOfSubdomainsCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            std::vector<std::vector<int>> verticesOfSubdomains = subdomainData->verticesOfSubdomains;

            checkEqual(verticesOfSubdomains[0].size(), 6u);
            checkEqual(verticesOfSubdomains[0][0], 0);
            checkEqual(verticesOfSubdomains[0][1], 2);
            checkEqual(verticesOfSubdomains[0][2], 3);
            checkEqual(verticesOfSubdomains[0][3], 1);
            checkEqual(verticesOfSubdomains[0][4], 5);
            checkEqual(verticesOfSubdomains[0][5], 4);

            checkEqual(verticesOfSubdomains[1].size(), 5u);
            checkEqual(verticesOfSubdomains[1][0], 1);
            checkEqual(verticesOfSubdomains[1][1], 4);
            checkEqual(verticesOfSubdomains[1][2], 5);
            checkEqual(verticesOfSubdomains[1][3], 0);
            checkEqual(verticesOfSubdomains[1][4], 3);
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            std::vector<std::vector<int>> verticesOfSubdomains = subdomainData->verticesOfSubdomains;

            checkEqual(verticesOfSubdomains.size(), 0u);
        }
    }
}

TestCase(ElementsOfSubdomainsCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            std::vector<std::vector<int>> elementsOfSubdomains = subdomainData->elementsOfSubdomains;

            checkEqual(elementsOfSubdomains[0].size(), 8u);
            checkEqual(elementsOfSubdomains[0][0], 0);
            checkEqual(elementsOfSubdomains[0][1], 1);
            checkEqual(elementsOfSubdomains[0][2], 2);
            checkEqual(elementsOfSubdomains[0][3], 3);
            checkEqual(elementsOfSubdomains[0][4], 4);
            checkEqual(elementsOfSubdomains[0][5], 7);
            checkEqual(elementsOfSubdomains[0][6], 8);
            checkEqual(elementsOfSubdomains[0][7], 9);

            checkEqual(elementsOfSubdomains[1].size(), 7u);
            checkEqual(elementsOfSubdomains[1][0], 0);
            checkEqual(elementsOfSubdomains[1][1], 2);
            checkEqual(elementsOfSubdomains[1][2], 3);
            checkEqual(elementsOfSubdomains[1][3], 4);
            checkEqual(elementsOfSubdomains[1][4], 5);
            checkEqual(elementsOfSubdomains[1][5], 6);
            checkEqual(elementsOfSubdomains[1][6], 7);
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            std::vector<std::vector<int>> elementsOfSubdomains = subdomainData->elementsOfSubdomains;

            checkEqual(elementsOfSubdomains.size(), 0u);
        }
    }
}

TestCase(GhostSolverIndicesCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            auto ghostGlobalIndices = subdomainData->ghostGlobalIndices;

            checkEqual(ghostGlobalIndices.size(), 3u);
            checkEqual(ghostGlobalIndices.at(0), 3);
            checkEqual(ghostGlobalIndices.at(1), 5);
            checkEqual(ghostGlobalIndices.at(2), 4);
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            auto ghostGlobalIndices = subdomainData->ghostGlobalIndices;

            checkEqual(ghostGlobalIndices.size(), 2u);
            checkEqual(ghostGlobalIndices.at(0), 0);
            checkEqual(ghostGlobalIndices.at(1), 2);
        }
    }
}

TestCase(LocalGridDataCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            GridDataPtr localGridData = subdomainData->localGridData;

            checkEqual(subdomainData->globalIndices.size(), 6u);
            checkEqual(subdomainData->globalIndices[0], 0);
            checkEqual(subdomainData->globalIndices[1], 1);
            checkEqual(subdomainData->globalIndices[2], 2);
            checkEqual(subdomainData->globalIndices[3], 3);
            checkEqual(subdomainData->globalIndices[4], 5);
            checkEqual(subdomainData->globalIndices[5], 4);
            checkEqual(localGridData->numberOfLocalVertices, 3);

            checkEqual(localGridData->coordinates.size(), 6u);
            checkClose(localGridData->coordinates[0][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[0][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[1][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[1][1], 1.0, TOLERANCE);
            checkClose(localGridData->coordinates[2][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[2][1], 1.0, TOLERANCE);
            checkClose(localGridData->coordinates[3][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[3][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[4][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[4][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[5][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[5][1], 1.0, TOLERANCE);

            {
                std::vector<std::vector<int>> expected{
                    {5, 0, 3, 2, 0},
                    {5, 0, 2, 1, 1},
                    {5, 3, 4, 2, 2},
                    {5, 4, 5, 2, 3},
                    {3, 0, 3, 4},
                    {3, 5, 2, 5},
                    {3, 2, 1, 6},
                    {3, 1, 0, 7}
                };

                auto connectivities = localGridData->connectivities;
                checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
                checkEqualCollections(connectivities[ 1].cbegin(), connectivities[ 1].cend(), expected[ 1].cbegin(), expected[ 1].cend());
                checkEqualCollections(connectivities[ 2].cbegin(), connectivities[ 2].cend(), expected[ 2].cbegin(), expected[ 2].cend());
                checkEqualCollections(connectivities[ 3].cbegin(), connectivities[ 3].cend(), expected[ 3].cbegin(), expected[ 3].cend());
                checkEqualCollections(connectivities[ 4].cbegin(), connectivities[ 4].cend(), expected[ 4].cbegin(), expected[ 4].cend());
                checkEqualCollections(connectivities[ 5].cbegin(), connectivities[ 5].cend(), expected[ 5].cbegin(), expected[ 5].cend());
                checkEqualCollections(connectivities[ 6].cbegin(), connectivities[ 6].cend(), expected[ 6].cbegin(), expected[ 6].cend());
                checkEqualCollections(connectivities[ 7].cbegin(), connectivities[ 7].cend(), expected[ 7].cbegin(), expected[ 7].cend());
            }

            {
                auto sections = localGridData->sections;
                checkEqual(sections.size(), 5u);

                checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 2);
                checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 3);

                {
                    auto section = sections[0];
                    checkEqual(section.name, std::string("A"));
                    checkEqual(section.dimension, 2);
                    checkEqual(section.begin, 0);
                    checkEqual(section.end, 2);
                    std::vector<int> expected{0, 1, 2, 3};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[1];
                    checkEqual(section.name, std::string("B"));
                    checkEqual(section.dimension, 2);
                    checkEqual(section.begin, 2);
                    checkEqual(section.end, 4);
                    std::vector<int> expected{2, 3, 4, 5};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[2];
                    checkEqual(section.name, std::string("SOUTH"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 4);
                    checkEqual(section.end, 5);
                    std::vector<int> expected{0, 3};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[3];
                    checkEqual(section.name, std::string("NORTH"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 5);
                    checkEqual(section.end, 7);
                    std::vector<int> expected{1, 2, 5};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[4];
                    checkEqual(section.name, std::string("WEST"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 7);
                    checkEqual(section.end, 8);
                    std::vector<int> expected{0, 1};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }
            }
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            GridDataPtr localGridData = subdomainData->localGridData;

            checkEqual(subdomainData->globalIndices.size(), 5u);
            checkEqual(subdomainData->globalIndices[0], 3);
            checkEqual(subdomainData->globalIndices[1], 4);
            checkEqual(subdomainData->globalIndices[2], 5);
            checkEqual(subdomainData->globalIndices[3], 0);
            checkEqual(subdomainData->globalIndices[4], 2);
            checkEqual(localGridData->numberOfLocalVertices, 3);

            checkEqual(localGridData->coordinates.size(), 5u);
            checkClose(localGridData->coordinates[0][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[0][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[1][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[1][1], 1.0, TOLERANCE);
            checkClose(localGridData->coordinates[2][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[2][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[3][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[3][1], 0.0, TOLERANCE);
            checkClose(localGridData->coordinates[4][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[4][1], 1.0, TOLERANCE);

            {
                std::vector<std::vector<int>> expected{
                    {5, 3, 0, 4, 0},
                    {5, 0, 2, 4, 1},
                    {5, 2, 1, 4, 2},
                    {3, 3, 0, 3},
                    {3, 0, 2, 4},
                    {3, 2, 1, 5},
                    {3, 1, 4, 6}
                };


                auto connectivities = localGridData->connectivities;
                checkEqualCollections(connectivities[ 0].cbegin(), connectivities[ 0].cend(), expected[ 0].cbegin(), expected[ 0].cend());
                checkEqualCollections(connectivities[ 1].cbegin(), connectivities[ 1].cend(), expected[ 1].cbegin(), expected[ 1].cend());
                checkEqualCollections(connectivities[ 2].cbegin(), connectivities[ 2].cend(), expected[ 2].cbegin(), expected[ 2].cend());
                checkEqualCollections(connectivities[ 3].cbegin(), connectivities[ 3].cend(), expected[ 3].cbegin(), expected[ 3].cend());
                checkEqualCollections(connectivities[ 4].cbegin(), connectivities[ 4].cend(), expected[ 4].cbegin(), expected[ 4].cend());
                checkEqualCollections(connectivities[ 5].cbegin(), connectivities[ 5].cend(), expected[ 5].cbegin(), expected[ 5].cend());
                checkEqualCollections(connectivities[ 6].cbegin(), connectivities[ 6].cend(), expected[ 6].cbegin(), expected[ 6].cend());
            }


            {
                auto sections = localGridData->sections;
                checkEqual(sections.size(), 5u);

                checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 2;}), 2);
                checkEqual(std::count_if(sections.cbegin(), sections.cend(), [](const auto& e){return e.dimension == 1;}), 3);

                {
                    auto section = sections[0];
                    checkEqual(section.name, std::string("A"));
                    checkEqual(section.dimension, 2);
                    checkEqual(section.begin, 0);
                    checkEqual(section.end, 1);
                    std::vector<int> expected{0, 3, 4};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[1];
                    checkEqual(section.name, std::string("B"));
                    checkEqual(section.dimension, 2);
                    checkEqual(section.begin, 1);
                    checkEqual(section.end, 3);
                    std::vector<int> expected{0, 1, 2, 4};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[2];
                    checkEqual(section.name, std::string("SOUTH"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 3);
                    checkEqual(section.end, 5);
                    std::vector<int> expected{0, 2, 3};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[3];
                    checkEqual(section.name, std::string("EAST"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 5);
                    checkEqual(section.end, 6);
                    std::vector<int> expected{1, 2};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }

                {
                    auto section = sections[4];
                    checkEqual(section.name, std::string("NORTH"));
                    checkEqual(section.dimension, 1);
                    checkEqual(section.begin, 6);
                    checkEqual(section.end, 7);
                    std::vector<int> expected{1, 4};
                    checkEqualCollections(section.vertices.cbegin(), section.vertices.cend(), expected.cbegin(), expected.cend());
                }
            }
        }
    }
}

TestSuiteEnd()
