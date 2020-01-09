#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include <cgnslib.h>

#define TOLERANCE 1.0e-12

struct DomainDivider3DFixture : public GridDataFixture {
    DomainDivider3DFixture() : GridDataFixture("14v_24e_3d.cgns") {}
};

FixtureTestSuite(DomainDivider3DSuite, DomainDivider3DFixture)

TestCase(VerticesOfSubdomainsCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            std::vector<std::vector<int>> verticesOfSubdomains = subdomainData->verticesOfSubdomains;

            checkEqual(verticesOfSubdomains[0].size(), 13u);
            checkEqual(verticesOfSubdomains[0][ 0],  1);
            checkEqual(verticesOfSubdomains[0][ 1],  4);
            checkEqual(verticesOfSubdomains[0][ 2],  5);
            checkEqual(verticesOfSubdomains[0][ 3],  6);
            checkEqual(verticesOfSubdomains[0][ 4],  9);
            checkEqual(verticesOfSubdomains[0][ 5], 10);
            checkEqual(verticesOfSubdomains[0][ 6], 13);
            checkEqual(verticesOfSubdomains[0][ 7], 12);
            checkEqual(verticesOfSubdomains[0][ 8], 11);
            checkEqual(verticesOfSubdomains[0][ 9],  8);
            checkEqual(verticesOfSubdomains[0][10],  7);
            checkEqual(verticesOfSubdomains[0][11],  2);

            checkEqual(verticesOfSubdomains[1].size(), 13u);
            checkEqual(verticesOfSubdomains[1][ 0],  0);
            checkEqual(verticesOfSubdomains[1][ 1],  2);
            checkEqual(verticesOfSubdomains[1][ 2],  3);
            checkEqual(verticesOfSubdomains[1][ 3],  7);
            checkEqual(verticesOfSubdomains[1][ 4],  8);
            checkEqual(verticesOfSubdomains[1][ 5], 11);
            checkEqual(verticesOfSubdomains[1][ 6], 12);
            checkEqual(verticesOfSubdomains[1][ 7], 13);
            checkEqual(verticesOfSubdomains[1][ 8],  9);
            checkEqual(verticesOfSubdomains[1][ 9], 10);
            checkEqual(verticesOfSubdomains[1][10],  4);
            checkEqual(verticesOfSubdomains[1][11],  1);
            checkEqual(verticesOfSubdomains[1][12],  6);
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

            checkEqual(elementsOfSubdomains[0].size(), 38u);
            checkEqual(elementsOfSubdomains[0][ 0],  0);
            checkEqual(elementsOfSubdomains[0][ 1],  1);
            checkEqual(elementsOfSubdomains[0][ 2],  2);
            checkEqual(elementsOfSubdomains[0][ 3],  3);
            checkEqual(elementsOfSubdomains[0][ 4],  5);
            checkEqual(elementsOfSubdomains[0][ 5],  6);
            checkEqual(elementsOfSubdomains[0][ 6],  7);
            checkEqual(elementsOfSubdomains[0][ 7],  9);
            checkEqual(elementsOfSubdomains[0][ 8], 10);
            checkEqual(elementsOfSubdomains[0][ 9], 11);
            checkEqual(elementsOfSubdomains[0][10], 13);
            checkEqual(elementsOfSubdomains[0][11], 14);
            checkEqual(elementsOfSubdomains[0][12], 15);
            checkEqual(elementsOfSubdomains[0][13], 17);
            checkEqual(elementsOfSubdomains[0][14], 18);
            checkEqual(elementsOfSubdomains[0][15], 19);
            checkEqual(elementsOfSubdomains[0][16], 20);
            checkEqual(elementsOfSubdomains[0][17], 21);
            checkEqual(elementsOfSubdomains[0][18], 22);
            checkEqual(elementsOfSubdomains[0][19], 23);
            checkEqual(elementsOfSubdomains[0][20], 25);
            checkEqual(elementsOfSubdomains[0][21], 27);
            checkEqual(elementsOfSubdomains[0][22], 28);
            checkEqual(elementsOfSubdomains[0][23], 29);
            checkEqual(elementsOfSubdomains[0][24], 30);
            checkEqual(elementsOfSubdomains[0][25], 31);
            checkEqual(elementsOfSubdomains[0][26], 32);
            checkEqual(elementsOfSubdomains[0][27], 33);
            checkEqual(elementsOfSubdomains[0][28], 34);
            checkEqual(elementsOfSubdomains[0][29], 35);
            checkEqual(elementsOfSubdomains[0][30], 37);
            checkEqual(elementsOfSubdomains[0][31], 39);
            checkEqual(elementsOfSubdomains[0][32], 40);
            checkEqual(elementsOfSubdomains[0][33], 42);
            checkEqual(elementsOfSubdomains[0][34], 44);
            checkEqual(elementsOfSubdomains[0][35], 45);
            checkEqual(elementsOfSubdomains[0][36], 46);
            checkEqual(elementsOfSubdomains[0][37], 47);

            checkEqual(elementsOfSubdomains[1].size(), 38u);
            checkEqual(elementsOfSubdomains[1][ 0],  0);
            checkEqual(elementsOfSubdomains[1][ 1],  1);
            checkEqual(elementsOfSubdomains[1][ 2],  2);
            checkEqual(elementsOfSubdomains[1][ 3],  3);
            checkEqual(elementsOfSubdomains[1][ 4],  4);
            checkEqual(elementsOfSubdomains[1][ 5],  5);
            checkEqual(elementsOfSubdomains[1][ 6],  7);
            checkEqual(elementsOfSubdomains[1][ 7],  8);
            checkEqual(elementsOfSubdomains[1][ 8],  9);
            checkEqual(elementsOfSubdomains[1][ 9], 11);
            checkEqual(elementsOfSubdomains[1][10], 12);
            checkEqual(elementsOfSubdomains[1][11], 14);
            checkEqual(elementsOfSubdomains[1][12], 15);
            checkEqual(elementsOfSubdomains[1][13], 16);
            checkEqual(elementsOfSubdomains[1][14], 17);
            checkEqual(elementsOfSubdomains[1][15], 18);
            checkEqual(elementsOfSubdomains[1][16], 19);
            checkEqual(elementsOfSubdomains[1][17], 21);
            checkEqual(elementsOfSubdomains[1][18], 22);
            checkEqual(elementsOfSubdomains[1][19], 23);
            checkEqual(elementsOfSubdomains[1][20], 24);
            checkEqual(elementsOfSubdomains[1][21], 25);
            checkEqual(elementsOfSubdomains[1][22], 26);
            checkEqual(elementsOfSubdomains[1][23], 27);
            checkEqual(elementsOfSubdomains[1][24], 28);
            checkEqual(elementsOfSubdomains[1][25], 30);
            checkEqual(elementsOfSubdomains[1][26], 32);
            checkEqual(elementsOfSubdomains[1][27], 33);
            checkEqual(elementsOfSubdomains[1][28], 36);
            checkEqual(elementsOfSubdomains[1][29], 37);
            checkEqual(elementsOfSubdomains[1][30], 38);
            checkEqual(elementsOfSubdomains[1][31], 39);
            checkEqual(elementsOfSubdomains[1][32], 40);
            checkEqual(elementsOfSubdomains[1][33], 41);
            checkEqual(elementsOfSubdomains[1][34], 42);
            checkEqual(elementsOfSubdomains[1][35], 43);
            checkEqual(elementsOfSubdomains[1][36], 45);
            checkEqual(elementsOfSubdomains[1][37], 47);
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

            checkEqual(ghostGlobalIndices.size(), 6u);
            checkEqual(ghostGlobalIndices.at(0), 13);
            checkEqual(ghostGlobalIndices.at(1), 12);
            checkEqual(ghostGlobalIndices.at(2), 11);
            checkEqual(ghostGlobalIndices.at(3), 10);
            checkEqual(ghostGlobalIndices.at(4),  8);
            checkEqual(ghostGlobalIndices.at(5),  7);
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            auto ghostGlobalIndices = subdomainData->ghostGlobalIndices;

            checkEqual(ghostGlobalIndices.size(), 6u);
            checkEqual(ghostGlobalIndices.at(0), 6);
            checkEqual(ghostGlobalIndices.at(1), 4);
            checkEqual(ghostGlobalIndices.at(2), 5);
            checkEqual(ghostGlobalIndices.at(3), 1);
            checkEqual(ghostGlobalIndices.at(4), 0);
            checkEqual(ghostGlobalIndices.at(5), 3);
        }
    }
}

TestCase(LocalGridDataCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
            GridDataPtr localGridData = subdomainData->localGridData;

            checkEqual(subdomainData->globalIndices.size(), 13u);
            checkEqual(subdomainData->globalIndices[ 0],  0);
            checkEqual(subdomainData->globalIndices[ 1],  1);
            checkEqual(subdomainData->globalIndices[ 2],  2);
            checkEqual(subdomainData->globalIndices[ 3],  3);
            checkEqual(subdomainData->globalIndices[ 4],  4);
            checkEqual(subdomainData->globalIndices[ 5],  5);
            checkEqual(subdomainData->globalIndices[ 6],  6);
            checkEqual(subdomainData->globalIndices[ 7], 13);
            checkEqual(subdomainData->globalIndices[ 8], 12);
            checkEqual(subdomainData->globalIndices[ 9], 11);
            checkEqual(subdomainData->globalIndices[10], 10);
            checkEqual(subdomainData->globalIndices[11],  8);
            checkEqual(subdomainData->globalIndices[12],  7);

            checkEqual(localGridData->numberOfLocalVertices, 7);

            checkEqual(localGridData->coordinates.size(), 13u);

            {
                auto connectivities = localGridData->connectivities;
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 20);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 18);
                checkEqual(connectivities.size(), 38u);
            }

            {
                auto entities = localGridData->entities;
                checkEqual(entities.size(), 7u);
                checkEqual(std::count_if(entities.cbegin(), entities.cend(), [](const auto& e){return e.dimension == 3;}), 1);
                checkEqual(std::count_if(entities.cbegin(), entities.cend(), [](const auto& e){return e.dimension == 2;}), 6);
            }
        }
        else {
            DomainDivider domainDivider;
            SubdomainDataPtr subdomainData = domainDivider.divideDomain();
            GridDataPtr localGridData = subdomainData->localGridData;

            checkEqual(subdomainData->globalIndices.size(), 13u);
            checkEqual(subdomainData->globalIndices[ 0],  7);
            checkEqual(subdomainData->globalIndices[ 1],  8);
            checkEqual(subdomainData->globalIndices[ 2],  9);
            checkEqual(subdomainData->globalIndices[ 3], 10);
            checkEqual(subdomainData->globalIndices[ 4], 11);
            checkEqual(subdomainData->globalIndices[ 5], 12);
            checkEqual(subdomainData->globalIndices[ 6], 13);
            checkEqual(subdomainData->globalIndices[ 7],  6);
            checkEqual(subdomainData->globalIndices[ 8],  4);
            checkEqual(subdomainData->globalIndices[ 9],  5);
            checkEqual(subdomainData->globalIndices[10],  1);
            checkEqual(subdomainData->globalIndices[11],  0);
            checkEqual(subdomainData->globalIndices[12],  3);

            checkEqual(localGridData->numberOfLocalVertices, 7);

            checkEqual(localGridData->coordinates.size(), 13u);

            {
                auto connectivities = localGridData->connectivities;
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 20);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 18);
                checkEqual(connectivities.size(), 38u);
            }

            {
                auto entities = localGridData->entities;
                checkEqual(entities.size(), 7u);
                checkEqual(std::count_if(entities.cbegin(), entities.cend(), [](const auto& e){return e.dimension == 3;}), 1);
                checkEqual(std::count_if(entities.cbegin(), entities.cend(), [](const auto& e){return e.dimension == 2;}), 6);
            }
        }
    }
}

TestSuiteEnd()
