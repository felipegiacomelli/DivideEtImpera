#ifndef __METIS_INTERFACE_GRAPH_BUILDER_HPP__
#define __METIS_INTERFACE_GRAPH_BUILDER_HPP__

#include <set>
#include <stdexcept>
#include <MSHtoCGNS/GridData/GridData.hpp>

class GraphBuilder {
    public:
        GraphBuilder() = default;

        std::vector<std::set<int>> buildGraph(GridDataPtr gridData);

        virtual ~GraphBuilder() = default;

    private:
        virtual std::vector<std::set<int>> buildGraph2D(GridDataPtr gridData);
        std::pair<int, int> getTriangleFaceNeighbours(const std::array<int, 4>& triangles, int face);
        std::pair<int, int> getQuadrangleFaceNeighbours(const std::array<int, 5>& quadrangles, int face);

        virtual std::vector<std::set<int>> buildGraph3D(GridDataPtr gridData);
        std::pair<int, int> getTetrahedronFaceNeighbours(const std::array<int, 5>& tetrahedrons, int face);
        std::pair<int, int> getHexahedronFaceNeighbours(const std::array<int, 9>& hexahedrons, int face);
        std::pair<int, int> getPrismFaceNeighbours(const std::array<int, 7>& prisms, int face);
        std::pair<int, int> getPyramidFaceNeighbours(const std::array<int, 6>& pyramids, int face);

        void addNeighbours(std::vector<std::set<int>>& neighbourhood, std::pair<int, int>&& neighbours);
};

#endif
