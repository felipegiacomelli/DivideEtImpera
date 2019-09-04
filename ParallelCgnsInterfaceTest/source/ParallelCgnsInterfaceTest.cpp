#define BOOST_TEST_MODULE ParallelCgnsInterfaceTestModule

#include <MSHtoCGNS/BoostInterface/Test.hpp>

#include "DivideEtImpera/UnitTest/ParallelFixture.hpp"

TestGlobalFixture(ParallelFixture);

#include <pcgnslib.h>

extern int pcg_mpi_initialized;

TestCase(AlternateBetweenParallelSequential) {
    boost::mpi::communicator world;
    std::string path = std::string(TEST_INPUT_DIRECTORY) + "/14v_24e_3d.cgns";
    int file;

    if (world.rank() == 0) {
        if (cg_open(path.c_str(), CG_MODE_READ, &file))
            cg_error_exit();

        if (cg_close(file))
            cg_error_exit();
    }

    {
        if (cgp_open(path.c_str(), CG_MODE_MODIFY, &file))
            cgp_error_exit();

        if (cgp_close(file))
            cgp_error_exit();
    }

    if (world.rank() == 0) {
        pcg_mpi_initialized = 0;
        cg_set_file_type(CG_FILE_HDF5);

        if (cg_open(path.c_str(), CG_MODE_READ, &file))
            cg_error_exit();

        if (cg_close(file))
            cg_error_exit();
    }

    {
        pcg_mpi_initialized = 1;

        if (cgp_open(path.c_str(), CG_MODE_MODIFY, &file))
            cgp_error_exit();

        if (cgp_close(file))
            cgp_error_exit();

        pcg_mpi_initialized = 0;
        cg_set_file_type(CG_FILE_HDF5);
    }
}
