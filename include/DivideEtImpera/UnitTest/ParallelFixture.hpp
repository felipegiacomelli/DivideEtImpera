#ifndef __UNIT_TEST_PARALLEL_FIXTURE_HPP__
#define __UNIT_TEST_PARALLEL_FIXTURE_HPP__

#include "DivideEtImpera/BoostInterface/Mpi.hpp"

struct ParallelFixture {
    boost::mpi::environment environment;
};

#endif
