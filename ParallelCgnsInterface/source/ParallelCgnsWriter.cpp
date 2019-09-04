#include "DivideEtImpera/ParallelCgnsInterface/ParallelCgnsWriter.hpp"
#include <pcgnslib.h>

extern int pcg_mpi_initialized;

ParallelCgnsWriter::ParallelCgnsWriter(std::string file, std::string gridLocation, std::pair<int, int> ownershipRange) : file(file) {
    pcg_mpi_initialized = 1;
    this->begin = ownershipRange.first + 1;
    this->end = ownershipRange.second + 1;
    this->checkFile();
    this->openFile();
    this->setGridLocation(gridLocation);
}

void ParallelCgnsWriter::checkFile() {
    if (!boost::filesystem::exists(this->file.parent_path()))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The parent path " + this->file.parent_path().string() + " does not exist");

    if (!boost::filesystem::exists(this->file))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - There is no file in the given path");

    if (cg_is_cgns(boost::filesystem::absolute(this->file).c_str(), &this->fileType))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The file is not a valid cgns file");

    if (this->fileType == 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Only HDF files are supported");
}

void ParallelCgnsWriter::openFile() {
    if (cgp_open(boost::filesystem::absolute(this->file).c_str(), CG_MODE_MODIFY, &this->fileIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not open the file " + boost::filesystem::absolute(this->file).string());

    if (cg_version(this->fileIndex, &this->fileVersion))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read file version");

    if (this->fileVersion <= 3.10)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - File version (" + std::to_string(this->fileVersion) + ") is older than 3.11");
}

void ParallelCgnsWriter::checkBase() {
    int numberOfBases;
    if (cg_nbases(this->fileIndex, &numberOfBases))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of bases");

    if (numberOfBases < 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has no base");
}

void ParallelCgnsWriter::checkZone() {
    int numberOfZones;
    if (cg_nzones(this->fileIndex, this->baseIndex, &numberOfZones))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read number of zones");

    if (numberOfZones < 1)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - The CGNS file has no zone");

    ZoneType_t zoneType;
    if (cg_zone_type(this->fileIndex, this->baseIndex, this->zoneIndex, &zoneType))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not read zone type");

    if (zoneType != Unstructured)
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Only unstructured zones are supported");
}

void ParallelCgnsWriter::setGridLocation(std::string gridLocation) {
    if (gridLocation == std::string("Vertex"))
        this->gridLocation = 2;
    else if (gridLocation == std::string("CellCenter"))
        this->gridLocation = 3;
    else
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Solution location must be either Vertex or CellCenter");
}

void ParallelCgnsWriter::writeSolution(std::string name) {
    this->name = name;
    this->writeSolution();
}

void ParallelCgnsWriter::writeSolution(double timeValue) {
    this->timeValues.push_back(timeValue);
    this->name = std::string("TimeStep") + std::to_string(timeValues.size());
    this->writeSolution();
}

void ParallelCgnsWriter::writeField(std::string name, const std::vector<double>& values) {
    this->name = name;
    this->dataType = 4;
    this->writeField();

    if (cgp_field_write_data(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, this->fieldIndex, &this->begin, &this->end, &values[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + name + " data");
}

void ParallelCgnsWriter::writeField(std::string name, const std::vector<int>& values) {
    this->name = name;
    this->dataType = 2;
    this->writeField();

    if (cgp_field_write_data(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, this->fieldIndex, &this->begin, &this->end, &values[0]))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + name + " data");
}

void ParallelCgnsWriter::writeSolution() {
    if (cg_sol_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->name.c_str(), GridLocation_t(this->gridLocation), &this->solutionIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write solution " + name);
}

void ParallelCgnsWriter::writeField() {
    if (cgp_field_write(this->fileIndex, this->baseIndex, this->zoneIndex, this->solutionIndex, DataType_t(this->dataType), this->name.c_str(), &this->fieldIndex))
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Could not write field " + name);
}

void ParallelCgnsWriter::finalizeTransient() {
    if (!this->isFinalized) {
        this->isFinalized = true;
        if (this->timeValues.size() > 0) {
            int numberOfTimeSteps = this->timeValues.size();
            cg_biter_write(this->fileIndex, this->baseIndex, "TimeIterativeValues", this->timeValues.size());
            cg_goto(this->fileIndex, this->baseIndex, "BaseIterativeData_t", 1, nullptr);
            cg_array_write("TimeValues", RealDouble, 1, &numberOfTimeSteps, &this->timeValues[0]);
            cg_simulation_type_write(this->fileIndex, this->baseIndex, TimeAccurate);
        }
    }
}

ParallelCgnsWriter::~ParallelCgnsWriter() {
    this->finalizeTransient();
    cgp_close(this->fileIndex);
    pcg_mpi_initialized = 0;
    cg_set_file_type(CG_FILE_HDF5);
}
