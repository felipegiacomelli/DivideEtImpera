#ifndef __METIS_INTERFACE_GRAPH_BUILDER_HPP__
#define __METIS_INTERFACE_GRAPH_BUILDER_HPP__

#include <stdexcept>
#include <set>
#include <MSHtoCGNS/GridData/GridData.hpp>

class GraphBuilder {
    public:
        GraphBuilder() = default;

        std::vector<std::set<int>> buildGraph(GridDataPtr gridData);

        virtual ~GraphBuilder() = default;

    private:
        std::vector<std::pair<int, int>> getTetrahedronFaceNeighbours(std::vector<std::vector<int>>::const_iterator tetrahedrons);
        std::vector<std::pair<int, int>> getHexahedronFaceNeighbours(std::vector<std::vector<int>>::const_iterator hexahedrons);
        std::vector<std::pair<int, int>> getPrismFaceNeighbours(std::vector<std::vector<int>>::const_iterator prisms);
        std::vector<std::pair<int, int>> getPyramidFaceNeighbours(std::vector<std::vector<int>>::const_iterator pyramids);
        std::vector<std::pair<int, int>> getTriangleFaceNeighbours(std::vector<std::vector<int>>::const_iterator triangles);
        std::vector<std::pair<int, int>> getQuadrangleFaceNeighbours(std::vector<std::vector<int>>::const_iterator quadrangles);

        void addNeighbours(std::vector<std::set<int>>& neighbourhood, std::vector<std::pair<int, int>>&& neighbours);
};

#endif
