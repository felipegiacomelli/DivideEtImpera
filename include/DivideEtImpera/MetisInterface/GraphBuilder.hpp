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
        std::vector<std::pair<int, int>> getFaceNeighboursTetr4(std::vector<std::vector<int>>::const_iterator tetrahedrons);
        std::vector<std::pair<int, int>> getFaceNeighboursHexa8(std::vector<std::vector<int>>::const_iterator hexahedrons);
        std::vector<std::pair<int, int>> getFaceNeighboursPenta6(std::vector<std::vector<int>>::const_iterator prisms);
        std::vector<std::pair<int, int>> getFaceNeighboursPyra5(std::vector<std::vector<int>>::const_iterator pyramids);
        std::vector<std::pair<int, int>> getFaceNeighboursTri3(std::vector<std::vector<int>>::const_iterator triangles);
        std::vector<std::pair<int, int>> getFaceNeighboursTri6(std::vector<std::vector<int>>::const_iterator triangles);
        std::vector<std::pair<int, int>> getFaceNeighboursQuad4(std::vector<std::vector<int>>::const_iterator quadrangles);

        void addNeighbours(std::vector<std::set<int>>& neighbourhood, std::vector<std::pair<int, int>>&& neighbours);
};

#endif
