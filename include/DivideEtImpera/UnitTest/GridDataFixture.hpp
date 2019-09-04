#ifndef __UNIT_TEST_GRID_DATA_FIXTURE_HPP__
#define __UNIT_TEST_GRID_DATA_FIXTURE_HPP__

#include <MSHtoCGNS/CgnsInterface/CgnsReader.hpp>

#include "DivideEtImpera/BoostInterface/Mpi.hpp"

struct GridDataFixture {
    GridDataFixture(std::string path) {
        if (this->world.rank() == 0) {
            CgnsReader cgnsReader(std::string(TEST_INPUT_DIRECTORY) + path);
            this->gridData = cgnsReader.gridData;
        }
    }

    boost::mpi::communicator world;
    GridDataPtr gridData = boost::make_shared<GridData>();
};

#endif
