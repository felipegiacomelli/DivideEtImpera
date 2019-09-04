#ifndef __PARALLEL_CGNS_INTERFACE_PARALLEL_CGNS_WRITER_HPP__
#define __PARALLEL_CGNS_INTERFACE_PARALLEL_CGNS_WRITER_HPP__

#include <string>
#include <set>

#include <MSHtoCGNS/BoostInterface/Filesystem.hpp>

class ParallelCgnsWriter {
    public:
        ParallelCgnsWriter() = default;
        ParallelCgnsWriter(std::string file, std::string gridLocation, std::pair<int, int> ownershipRange);

        void writeSolution(std::string name);
        void writeSolution(double timeValue);

        void writeField(std::string name, const std::vector<double>& values);
        void writeField(std::string name, const std::vector<int>& values);

        void finalizeTransient();

        ~ParallelCgnsWriter();

    private:
        void checkFile();
        void openFile();
        void checkBase();
        void checkZone();
        void setGridLocation(std::string gridLocation);

        void writeSolution();
        void writeField();

        boost::filesystem::path file;
        int fileType;
        int fileIndex;
        float fileVersion;
        int baseIndex = 1, zoneIndex = 1;

        int gridLocation;
        int begin, end;

        std::string name;
        int dataType;
        int solutionIndex, fieldIndex;
        std::vector<double> timeValues;
        bool isFinalized = false;
};

#endif
