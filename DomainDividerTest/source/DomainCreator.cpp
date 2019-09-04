#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/DomainDividerFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainCreator.hpp"

#define TOLERANCE 1.0e-6

struct DomainCreatorFixture : public DomainDividerFixture {
    DomainCreatorFixture() : DomainDividerFixture("27v_28e_2d.cgns") {
        if (this->world.rank() == 0)
            if (!boost::filesystem::exists(this->output))
                boost::filesystem::create_directory(this->output);
    }

    ~DomainCreatorFixture() {
        if (this->world.rank() == 0)
            boost::filesystem::remove_all(this->output);
    }

    std::string output = "./DomainCreatorFixture";
};

FixtureTestSuite(DomainCreatorSuite, DomainCreatorFixture)

TestCase(DomainCreatorCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            DomainCreator domainCreator(this->gridData, this->subdomainData, this->output);

            CgnsReader cgnsReader(domainCreator.getFileName());

            checkEqual(cgnsReader.gridData->triangles.size(), this->gridData->triangles.size());
            checkEqual(cgnsReader.gridData->quadrangles.size(), this->gridData->quadrangles.size());
            checkEqual(cgnsReader.gridData->lines.size(), this->gridData->lines.size());
            checkEqual(cgnsReader.gridData->regions.size(), this->gridData->regions.size());
            checkEqual(cgnsReader.gridData->boundaries.size(), this->gridData->boundaries.size());

            auto coordinates = cgnsReader.gridData->coordinates;
            checkEqual(coordinates.size(), this->gridData->coordinates.size());
            checkClose(coordinates[ 0][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 0][1], 0.00000000e+00, TOLERANCE); checkSmall(coordinates[ 0][2], TOLERANCE);
            checkClose(coordinates[ 1][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 1][1], 0.00000000e+00, TOLERANCE); checkSmall(coordinates[ 1][2], TOLERANCE);
            checkClose(coordinates[ 2][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 2][1], 5.00000000e-01, TOLERANCE); checkSmall(coordinates[ 2][2], TOLERANCE);
            checkClose(coordinates[ 3][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 3][1], 5.00000000e-01, TOLERANCE); checkSmall(coordinates[ 3][2], TOLERANCE);
            checkClose(coordinates[ 4][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 4][1], 1.00000000e+00, TOLERANCE); checkSmall(coordinates[ 4][2], TOLERANCE);
            checkClose(coordinates[ 5][0], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 5][1], 0.00000000e+00, TOLERANCE); checkSmall(coordinates[ 5][2], TOLERANCE);
            checkClose(coordinates[ 6][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 6][1], 7.50000000e-01, TOLERANCE); checkSmall(coordinates[ 6][2], TOLERANCE);
            checkClose(coordinates[ 7][0], 7.50000000e-01, TOLERANCE); checkClose(coordinates[ 7][1], 1.00000000e+00, TOLERANCE); checkSmall(coordinates[ 7][2], TOLERANCE);
            checkClose(coordinates[ 8][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 8][1], 2.50000000e-01, TOLERANCE); checkSmall(coordinates[ 8][2], TOLERANCE);
            checkClose(coordinates[ 9][0], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 9][1], 5.00000000e-01, TOLERANCE); checkSmall(coordinates[ 9][2], TOLERANCE);
            checkClose(coordinates[10][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[10][1], 7.50000000e-01, TOLERANCE); checkSmall(coordinates[10][2], TOLERANCE);
            checkClose(coordinates[11][0], 6.71866414e-01, TOLERANCE); checkClose(coordinates[11][1], 6.71866414e-01, TOLERANCE); checkSmall(coordinates[11][2], TOLERANCE);
            checkClose(coordinates[12][0], 7.49999999e-01, TOLERANCE); checkClose(coordinates[12][1], 7.50000000e-01, TOLERANCE); checkSmall(coordinates[12][2], TOLERANCE);
            checkClose(coordinates[13][0], 7.47023810e-01, TOLERANCE); checkClose(coordinates[13][1], 2.52976190e-01, TOLERANCE); checkSmall(coordinates[13][2], TOLERANCE);
            checkClose(coordinates[14][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[14][1], 0.00000000e+00, TOLERANCE); checkSmall(coordinates[14][2], TOLERANCE);
            checkClose(coordinates[15][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[15][1], 5.00000000e-01, TOLERANCE); checkSmall(coordinates[15][2], TOLERANCE);
            checkClose(coordinates[16][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[16][1], 1.00000000e+00, TOLERANCE); checkSmall(coordinates[16][2], TOLERANCE);
            checkClose(coordinates[17][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[17][1], 1.00000000e+00, TOLERANCE); checkSmall(coordinates[17][2], TOLERANCE);
            checkClose(coordinates[18][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[18][1], 2.50000000e-01, TOLERANCE); checkSmall(coordinates[18][2], TOLERANCE);
            checkClose(coordinates[19][0], 7.50000000e-01, TOLERANCE); checkClose(coordinates[19][1], 5.00000000e-01, TOLERANCE); checkSmall(coordinates[19][2], TOLERANCE);
            checkClose(coordinates[20][0], 8.28134659e-01, TOLERANCE); checkClose(coordinates[20][1], 8.28134660e-01, TOLERANCE); checkSmall(coordinates[20][2], TOLERANCE);
            checkClose(coordinates[21][0], 2.52976190e-01, TOLERANCE); checkClose(coordinates[21][1], 7.47023810e-01, TOLERANCE); checkSmall(coordinates[21][2], TOLERANCE);
            checkClose(coordinates[22][0], 3.75744048e-01, TOLERANCE); checkClose(coordinates[22][1], 6.24255952e-01, TOLERANCE); checkSmall(coordinates[22][2], TOLERANCE);
            checkClose(coordinates[23][0], 2.82368215e-01, TOLERANCE); checkClose(coordinates[23][1], 2.23366201e-01, TOLERANCE); checkSmall(coordinates[23][2], TOLERANCE);
            checkClose(coordinates[24][0], 1.17744630e-01, TOLERANCE); checkClose(coordinates[24][1], 2.90032619e-01, TOLERANCE); checkSmall(coordinates[24][2], TOLERANCE);
            checkClose(coordinates[25][0], 1.43625351e-01, TOLERANCE); checkClose(coordinates[25][1], 1.96686762e-01, TOLERANCE); checkSmall(coordinates[25][2], TOLERANCE);
            checkClose(coordinates[26][0], 6.24255952e-01, TOLERANCE); checkClose(coordinates[26][1], 3.75744048e-01, TOLERANCE); checkSmall(coordinates[26][2], TOLERANCE);
        }
    }
}

TestSuiteEnd()
