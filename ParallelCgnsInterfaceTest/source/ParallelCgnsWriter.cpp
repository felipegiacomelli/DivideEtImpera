#include <MSHtoCGNS/BoostInterface/Test.hpp>
#include "DivideEtImpera/UnitTest/DomainDividerFixture.hpp"
#include "DivideEtImpera/DomainDivider/DomainCreator.hpp"
#include "DivideEtImpera/ParallelCgnsInterface/ParallelCgnsWriter.hpp"

#include <cgnslib.h>

#define TOLERANCE 1.0e-12

struct ParallelCgnsWriterFixture : public DomainDividerFixture {
    ParallelCgnsWriterFixture() : DomainDividerFixture("29v_39e_3d.cgns") {
        if (this->world.rank() == 0) {
            DomainCreator domainCreator(this->gridData, this->subdomainData, "./");
            this->output = domainCreator.getFileName();
        }
        boost::mpi::broadcast(this->world, this->output, 0);
    }

    ~ParallelCgnsWriterFixture() {
        if (this->world.rank() == 0)
            boost::filesystem::remove_all(boost::filesystem::path(this->output).parent_path());
    }

    std::string output;
    int fileIndex, baseIndex = 1, zoneIndex = 1;
    char buffer[1024];
    GridLocation_t location;
    DataType_t datatype;
    int range_min = 1, range_max = 29;
    double field[29];
};

FixtureTestSuite(ParallelCgnsWriterSuite, ParallelCgnsWriterFixture)

TestCase(ParallelCgnsWriterCase) {
    if (this->world.size() == 2) {
        auto localGridData{this->subdomainData->localGridData};
        ParallelCgnsWriter parallelCgnsWriter(this->output, "Vertex", this->subdomainData->ownershipRange);
        parallelCgnsWriter.writeSolution("Solution");
        parallelCgnsWriter.writeField("Subdomain", std::vector<double>(localGridData->numberOfLocalVertices, this->world.rank() + 1));
    }

    if (this->world.size() == 2) {
        if (this->world.rank() == 0) {
            cg_open(this->output.c_str(), CG_MODE_READ, &this->fileIndex);
            int numberOfSolutions;
            cg_nsols(this->fileIndex, this->baseIndex, this->zoneIndex, &numberOfSolutions);
            checkEqual(numberOfSolutions, 1);

            int solutionNumber = 1;
            cg_sol_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, &this->location);
            checkEqual(this->buffer, "Solution");
            checkEqual(location, Vertex);

            int numberOfFields;
            cg_nfields(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, &numberOfFields);
            checkEqual(numberOfFields, 1);

            int fieldNumber = 1;
            cg_field_info(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, fieldNumber, &this->datatype, this->buffer);
            checkEqual(this->buffer, "Subdomain");
            checkEqual(this->datatype, RealDouble);

            cg_field_read(this->fileIndex, this->baseIndex, this->zoneIndex, solutionNumber, this->buffer, this->datatype, &this->range_min, &this->range_max, this->field);

            for (int j = 0; j < 15; ++j)
                checkClose(field[j], 1.0, TOLERANCE);

            for (int j = 15; j < 29; ++j)
                checkClose(field[j], 2.0, TOLERANCE);
        }
    }
}

TestSuiteEnd()
