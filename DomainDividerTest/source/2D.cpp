#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/GridDataFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"

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

// TestCase(LocalGridDataCase) {
//     if (this->world.size() == 2) {
//         if (this->world.rank() == 0) {
//             DomainDivider domainDivider;
//             SubdomainDataPtr subdomainData = domainDivider.divideDomain(this->gridData);
//             GridDataPtr localGridData = subdomainData->localGridData;

//             checkEqual(subdomainData->globalIndices.size(), 6u);
//             checkEqual(subdomainData->globalIndices[0], 0);
//             checkEqual(subdomainData->globalIndices[1], 1);
//             checkEqual(subdomainData->globalIndices[2], 2);
//             checkEqual(subdomainData->globalIndices[3], 3);
//             checkEqual(subdomainData->globalIndices[4], 5);
//             checkEqual(subdomainData->globalIndices[5], 4);
//             checkEqual(localGridData->numberOfLocalVertices, 3);

//             checkEqual(localGridData->coordinates.size(), 6u);
//             checkClose(localGridData->coordinates[0][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[0][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[1][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[1][1], 1.0, TOLERANCE);
//             checkClose(localGridData->coordinates[2][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[2][1], 1.0, TOLERANCE);
//             checkClose(localGridData->coordinates[3][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[3][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[4][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[4][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[5][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[5][1], 1.0, TOLERANCE);

//             checkEqual(localGridData->triangles.size(), 4u);
//             checkEqual(localGridData->triangles[0][0], 0); checkEqual(localGridData->triangles[0][1], 3); checkEqual(localGridData->triangles[0][2], 2); checkEqual(localGridData->triangles[0][3], 0);
//             checkEqual(localGridData->triangles[1][0], 0); checkEqual(localGridData->triangles[1][1], 2); checkEqual(localGridData->triangles[1][2], 1); checkEqual(localGridData->triangles[1][3], 1);
//             checkEqual(localGridData->triangles[2][0], 3); checkEqual(localGridData->triangles[2][1], 4); checkEqual(localGridData->triangles[2][2], 2); checkEqual(localGridData->triangles[2][3], 2);
//             checkEqual(localGridData->triangles[3][0], 4); checkEqual(localGridData->triangles[3][1], 5); checkEqual(localGridData->triangles[3][2], 2); checkEqual(localGridData->triangles[3][3], 3);

//             checkEqual(localGridData->lines.size(), 4u);
//             checkEqual(localGridData->lines[0][0], 0); checkEqual(localGridData->lines[0][1], 3); checkEqual(localGridData->lines[0][2], 4);
//             checkEqual(localGridData->lines[1][0], 5); checkEqual(localGridData->lines[1][1], 2); checkEqual(localGridData->lines[1][2], 5);
//             checkEqual(localGridData->lines[2][0], 2); checkEqual(localGridData->lines[2][1], 1); checkEqual(localGridData->lines[2][2], 6);
//             checkEqual(localGridData->lines[3][0], 1); checkEqual(localGridData->lines[3][1], 0); checkEqual(localGridData->lines[3][2], 7);

//             checkEqual(localGridData->regions.size(), 2u);
//             checkEqual(localGridData->boundaries.size(), 3u);

//             auto region = localGridData->regions[0];
//             checkEqual(region.name, "A"); checkEqual(region.begin, 0); checkEqual(region.end, 2);
//             checkEqual(region.vertices.size(), 4u);
//             checkEqual(region.vertices[0], 0);
//             checkEqual(region.vertices[1], 1);
//             checkEqual(region.vertices[2], 2);
//             checkEqual(region.vertices[3], 3);

//             region = localGridData->regions[1];
//             checkEqual(region.name, "B"); checkEqual(region.begin, 2); checkEqual(region.end, 4);
//             checkEqual(region.vertices.size(), 4u);
//             checkEqual(region.vertices[0], 2);
//             checkEqual(region.vertices[1], 3);
//             checkEqual(region.vertices[2], 4);
//             checkEqual(region.vertices[3], 5);

//             auto boundary = localGridData->boundaries[0];
//             checkEqual(boundary.name, "SOUTH"); checkEqual(boundary.begin, 4); checkEqual(boundary.end, 5);
//             checkEqual(boundary.vertices.size(), 2u);
//             checkEqual(boundary.vertices[0], 0);
//             checkEqual(boundary.vertices[1], 3);

//             boundary = localGridData->boundaries[1];
//             checkEqual(boundary.name, "NORTH"); checkEqual(boundary.begin, 5); checkEqual(boundary.end, 7);
//             checkEqual(boundary.vertices.size(), 3u);
//             checkEqual(boundary.vertices[0], 1);
//             checkEqual(boundary.vertices[1], 2);
//             checkEqual(boundary.vertices[2], 5);

//             boundary = localGridData->boundaries[2];
//             checkEqual(boundary.name, "WEST");  checkEqual(boundary.begin, 7); checkEqual(boundary.end, 8);
//             checkEqual(boundary.vertices.size(), 2u);
//             checkEqual(boundary.vertices[0], 0);
//             checkEqual(boundary.vertices[1], 1);
//         }
//         else {
//             DomainDivider domainDivider;
//             SubdomainDataPtr subdomainData = domainDivider.divideDomain();
//             GridDataPtr localGridData = subdomainData->localGridData;

//             checkEqual(subdomainData->globalIndices.size(), 5u);
//             checkEqual(subdomainData->globalIndices[0], 3);
//             checkEqual(subdomainData->globalIndices[1], 4);
//             checkEqual(subdomainData->globalIndices[2], 5);
//             checkEqual(subdomainData->globalIndices[3], 0);
//             checkEqual(subdomainData->globalIndices[4], 2);
//             checkEqual(localGridData->numberOfLocalVertices, 3);

//             checkEqual(localGridData->coordinates.size(), 5u);
//             checkClose(localGridData->coordinates[0][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[0][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[1][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[1][1], 1.0, TOLERANCE);
//             checkClose(localGridData->coordinates[2][0], 2.0, TOLERANCE); checkClose(localGridData->coordinates[2][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[3][0], 0.0, TOLERANCE); checkClose(localGridData->coordinates[3][1], 0.0, TOLERANCE);
//             checkClose(localGridData->coordinates[4][0], 1.0, TOLERANCE); checkClose(localGridData->coordinates[4][1], 1.0, TOLERANCE);

//             checkEqual(localGridData->triangles.size(), 3u);
//             checkEqual(localGridData->triangles[0][0], 3); checkEqual(localGridData->triangles[0][1], 0); checkEqual(localGridData->triangles[0][2], 4); checkEqual(localGridData->triangles[0][3], 0);
//             checkEqual(localGridData->triangles[1][0], 0); checkEqual(localGridData->triangles[1][1], 2); checkEqual(localGridData->triangles[1][2], 4); checkEqual(localGridData->triangles[1][3], 1);
//             checkEqual(localGridData->triangles[2][0], 2); checkEqual(localGridData->triangles[2][1], 1); checkEqual(localGridData->triangles[2][2], 4); checkEqual(localGridData->triangles[2][3], 2);

//             checkEqual(localGridData->lines.size(), 4u);
//             checkEqual(localGridData->lines[0][0], 3); checkEqual(localGridData->lines[0][1], 0); checkEqual(localGridData->lines[0][2], 3);
//             checkEqual(localGridData->lines[1][0], 0); checkEqual(localGridData->lines[1][1], 2); checkEqual(localGridData->lines[1][2], 4);
//             checkEqual(localGridData->lines[2][0], 2); checkEqual(localGridData->lines[2][1], 1); checkEqual(localGridData->lines[2][2], 5);
//             checkEqual(localGridData->lines[3][0], 1); checkEqual(localGridData->lines[3][1], 4); checkEqual(localGridData->lines[3][2], 6);

//             checkEqual(localGridData->regions.size(), 2u);
//             checkEqual(localGridData->boundaries.size(), 3u);

//             auto region = localGridData->regions[0];
//             checkEqual(region.name, "A"); checkEqual(region.begin, 0); checkEqual(region.end, 1);
//             checkEqual(region.vertices.size(), 3u);
//             checkEqual(region.vertices[0], 0);
//             checkEqual(region.vertices[1], 3);
//             checkEqual(region.vertices[2], 4);

//             region = localGridData->regions[1];
//             checkEqual(region.name, "B"); checkEqual(region.begin, 1); checkEqual(region.end, 3);
//             checkEqual(region.vertices.size(), 4u);
//             checkEqual(region.vertices[0], 0);
//             checkEqual(region.vertices[1], 1);
//             checkEqual(region.vertices[2], 2);
//             checkEqual(region.vertices[3], 4);

//             auto boundary = localGridData->boundaries[0];
//             checkEqual(boundary.name, "SOUTH"); checkEqual(boundary.begin, 3); checkEqual(boundary.end, 5);
//             checkEqual(boundary.vertices.size(), 3u);
//             checkEqual(boundary.vertices[0], 0);
//             checkEqual(boundary.vertices[1], 2);
//             checkEqual(boundary.vertices[2], 3);

//             boundary = localGridData->boundaries[1];
//             checkEqual(boundary.name, "EAST");  checkEqual(boundary.begin, 5); checkEqual(boundary.end, 6);
//             checkEqual(boundary.vertices.size(), 2u);
//             checkEqual(boundary.vertices[0], 1);
//             checkEqual(boundary.vertices[1], 2);

//             boundary = localGridData->boundaries[2];
//             checkEqual(boundary.name, "NORTH"); checkEqual(boundary.begin, 6); checkEqual(boundary.end, 7);
//             checkEqual(boundary.vertices.size(), 2u);
//             checkEqual(boundary.vertices[0], 1);
//             checkEqual(boundary.vertices[1], 4);
//         }
//     }
// }

TestSuiteEnd()
