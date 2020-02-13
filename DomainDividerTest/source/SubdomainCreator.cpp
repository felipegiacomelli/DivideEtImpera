#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/DomainDividerFixture.hpp"
#include "DivideEtImpera/DomainDivider/SubdomainCreator.hpp"
#include <cgnslib.h>

#define TOLERANCE 1.0e-6

struct SubdomainCreatorFixture : public DomainDividerFixture {
    SubdomainCreatorFixture() : DomainDividerFixture("29v_39e_3d.cgns") {
        if (this->world.rank() == 0)
            if (!boost::filesystem::exists(this->output))
                boost::filesystem::create_directory(this->output);
    }

    ~SubdomainCreatorFixture() {
        if (this->world.rank() == 0)
            boost::filesystem::remove_all(this->output);
    }

    std::string output = "./SubdomainCreatorFixture";
};

FixtureTestSuite(SubdomainCreatorSuite, SubdomainCreatorFixture)

TestCase(SubdomainCreatorCase) {
    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            SubdomainCreator subdomainCreator(this->subdomainData->localGridData, this->output);
            CgnsReader cgnsReader(subdomainCreator.getFileName());

            {
                auto connectivities = cgnsReader.gridData->connectivities;
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 4);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == PENTA_6;}), 2);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 8);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == QUAD_4 ;}), 4);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == BAR_2  ;}), 1);
                checkEqual(connectivities.size(), 19u);
            }

            auto coordinates = cgnsReader.gridData->coordinates;
            checkEqual(coordinates.size(), 15u);
            checkClose(coordinates[ 0][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 0][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 0][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 1][0], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 1][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 1][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 2][0], 1.76776695e-01, TOLERANCE); checkClose(coordinates[ 2][1], 1.76776695e-01, TOLERANCE); checkClose(coordinates[ 2][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 3][0], 1.53080850e-17, TOLERANCE); checkClose(coordinates[ 3][1], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 3][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 4][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 4][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 4][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 5][0], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 5][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 5][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 6][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 6][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 6][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 7][0], 1.76776695e-01, TOLERANCE); checkClose(coordinates[ 7][1], 1.76776695e-01, TOLERANCE); checkClose(coordinates[ 7][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 8][0], 3.53553391e-01, TOLERANCE); checkClose(coordinates[ 8][1], 3.53553391e-01, TOLERANCE); checkClose(coordinates[ 8][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 9][0], 1.53080850e-17, TOLERANCE); checkClose(coordinates[ 9][1], 2.50000000e-01, TOLERANCE); checkClose(coordinates[ 9][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[10][0], 3.06161700e-17, TOLERANCE); checkClose(coordinates[10][1], 5.00000000e-01, TOLERANCE); checkClose(coordinates[10][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[11][0], 6.55386666e-01, TOLERANCE); checkClose(coordinates[11][1], 2.71470046e-01, TOLERANCE); checkClose(coordinates[11][2], 5.00000000e-01, TOLERANCE);
            checkClose(coordinates[12][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[12][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[12][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[13][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[13][1], 5.00000000e-01, TOLERANCE); checkClose(coordinates[13][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[14][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[14][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[14][2], 5.00000000e-01, TOLERANCE);
        }
        else {
            SubdomainCreator subdomainCreator(this->subdomainData->localGridData, this->output);
            CgnsReader cgnsReader(subdomainCreator.getFileName());

            {
                auto connectivities = cgnsReader.gridData->connectivities;
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TETRA_4;}), 10);
                checkEqual(std::count_if(connectivities.cbegin(), connectivities.cend(), [](const auto& c){return c.front() == TRI_3  ;}), 14);
                checkEqual(connectivities.size(), 24u);
            }

            auto coordinates = cgnsReader.gridData->coordinates;
            checkEqual(cgnsReader.gridData->coordinates.size(), 14u);
            checkClose(coordinates[ 0][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 0][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 0][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 1][0], 3.53553391e-01, TOLERANCE); checkClose(coordinates[ 1][1], 3.53553391e-01, TOLERANCE); checkClose(coordinates[ 1][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 2][0], 3.06161700e-17, TOLERANCE); checkClose(coordinates[ 2][1], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 2][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 3][0], 2.71470046e-01, TOLERANCE); checkClose(coordinates[ 3][1], 6.55386666e-01, TOLERANCE); checkClose(coordinates[ 3][2], 5.00000000e-01, TOLERANCE);
            checkClose(coordinates[ 4][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 4][1], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 4][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 5][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 5][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 5][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 6][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 6][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 6][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 7][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 7][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 7][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 8][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 8][1], 5.00000000e-01, TOLERANCE); checkClose(coordinates[ 8][2], 0.00000000e+00, TOLERANCE);
            checkClose(coordinates[ 9][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[ 9][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[ 9][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[10][0], 5.00000000e-01, TOLERANCE); checkClose(coordinates[10][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[10][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[11][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[11][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[11][2], 1.00000000e+00, TOLERANCE);
            checkClose(coordinates[12][0], 0.00000000e+00, TOLERANCE); checkClose(coordinates[12][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[12][2], 5.00000000e-01, TOLERANCE);
            checkClose(coordinates[13][0], 1.00000000e+00, TOLERANCE); checkClose(coordinates[13][1], 1.00000000e+00, TOLERANCE); checkClose(coordinates[13][2], 5.00000000e-01, TOLERANCE);
        }
    }
}

TestSuiteEnd()
