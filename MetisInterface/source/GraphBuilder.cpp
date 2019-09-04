#include "DivideEtImpera/MetisInterface/GraphBuilder.hpp"

std::vector<std::set<int>> GraphBuilder::buildGraph(GridDataPtr gridData) {
    switch (gridData->dimension) {
        case 2: return this->buildGraph2D(gridData);
        case 3: return this->buildGraph3D(gridData);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid gridData dimension " + std::to_string(gridData->dimension));
    }
}

std::vector<std::set<int>> GraphBuilder::buildGraph2D(GridDataPtr gridData) {
    std::vector<std::set<int>> neighbourhood(gridData->coordinates.size());
    for (const auto& triangle : gridData->triangles) {
        for (int f = 0; f < 3; ++f) {
            this->addNeighbours(neighbourhood, this->getTriangleFaceNeighbours(triangle, f));
        }
    }
    for (const auto& quadrangle : gridData->quadrangles) {
        for (int f = 0; f < 4; ++f) {
            this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(quadrangle, f));
        }
    }
    return neighbourhood;
}

std::vector<std::set<int>> GraphBuilder::buildGraph3D(GridDataPtr gridData) {
    std::vector<std::set<int>> neighbourhood(gridData->coordinates.size());
    for (const auto& tetrahedron : gridData->tetrahedrons) {
        for (int f = 0; f < 6; ++f) {
            this->addNeighbours(neighbourhood, this->getTetrahedronFaceNeighbours(tetrahedron, f));
        }
    }
    for (const auto& hexahedron : gridData->hexahedrons) {
        for (int f = 0; f < 12; ++f) {
            this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(hexahedron, f));
        }
    }
    for (const auto& prism : gridData->prisms) {
        for (int f = 0; f < 9; ++f) {
            this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(prism, f));
        }
    }
    for (const auto& pyramid : gridData->pyramids) {
        for (int f = 0; f < 8; ++f) {
            this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(pyramid, f));
        }
    }
    return neighbourhood;
}

std::pair<int, int> GraphBuilder::getTriangleFaceNeighbours(const std::array<int, 4>& triangle, int face) {
    switch(face) {
        case 0: return std::make_pair(triangle[0], triangle[1]);
        case 1: return std::make_pair(triangle[1], triangle[2]);
        case 2: return std::make_pair(triangle[2], triangle[0]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

std::pair<int, int> GraphBuilder::getQuadrangleFaceNeighbours(const std::array<int, 5>& quadrangle, int face) {
    switch(face) {
        case 0: return std::make_pair(quadrangle[0], quadrangle[1]);
        case 1: return std::make_pair(quadrangle[1], quadrangle[2]);
        case 2: return std::make_pair(quadrangle[2], quadrangle[3]);
        case 3: return std::make_pair(quadrangle[3], quadrangle[0]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

std::pair<int, int> GraphBuilder::getTetrahedronFaceNeighbours(const std::array<int, 5>& tetrahedron, int face) {
    switch(face) {
        case 0: return std::make_pair(tetrahedron[0], tetrahedron[1]);
        case 1: return std::make_pair(tetrahedron[1], tetrahedron[2]);
        case 2: return std::make_pair(tetrahedron[2], tetrahedron[0]);
        case 3: return std::make_pair(tetrahedron[0], tetrahedron[3]);
        case 4: return std::make_pair(tetrahedron[1], tetrahedron[3]);
        case 5: return std::make_pair(tetrahedron[2], tetrahedron[3]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

std::pair<int, int> GraphBuilder::getHexahedronFaceNeighbours(const std::array<int, 9>& hexahedron, int face) {
    switch(face) {
        case  0: return std::make_pair(hexahedron[0], hexahedron[1]);
        case  1: return std::make_pair(hexahedron[1], hexahedron[2]);
        case  2: return std::make_pair(hexahedron[2], hexahedron[3]);
        case  3: return std::make_pair(hexahedron[3], hexahedron[0]);
        case  4: return std::make_pair(hexahedron[4], hexahedron[5]);
        case  5: return std::make_pair(hexahedron[5], hexahedron[6]);
        case  6: return std::make_pair(hexahedron[6], hexahedron[7]);
        case  7: return std::make_pair(hexahedron[7], hexahedron[4]);
        case  8: return std::make_pair(hexahedron[4], hexahedron[0]);
        case  9: return std::make_pair(hexahedron[5], hexahedron[1]);
        case 10: return std::make_pair(hexahedron[6], hexahedron[2]);
        case 11: return std::make_pair(hexahedron[7], hexahedron[3]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

std::pair<int, int> GraphBuilder::getPrismFaceNeighbours(const std::array<int, 7>& prism, int face) {
    switch(face) {
        case 0: return std::make_pair(prism[0], prism[1]);
        case 1: return std::make_pair(prism[1], prism[2]);
        case 2: return std::make_pair(prism[2], prism[0]);
        case 3: return std::make_pair(prism[4], prism[3]);
        case 4: return std::make_pair(prism[5], prism[4]);
        case 5: return std::make_pair(prism[3], prism[5]);
        case 6: return std::make_pair(prism[3], prism[0]);
        case 7: return std::make_pair(prism[4], prism[1]);
        case 8: return std::make_pair(prism[5], prism[2]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

std::pair<int, int> GraphBuilder::getPyramidFaceNeighbours(const std::array<int, 6>& pyramid, int face) {
    switch(face) {
        case 0: return std::make_pair(pyramid[0], pyramid[1]);
        case 1: return std::make_pair(pyramid[1], pyramid[2]);
        case 2: return std::make_pair(pyramid[2], pyramid[3]);
        case 3: return std::make_pair(pyramid[3], pyramid[0]);
        case 4: return std::make_pair(pyramid[0], pyramid[4]);
        case 5: return std::make_pair(pyramid[1], pyramid[4]);
        case 6: return std::make_pair(pyramid[2], pyramid[4]);
        case 7: return std::make_pair(pyramid[3], pyramid[4]);
        default: throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Invalid face number " + std::to_string(face));
    }
}

void GraphBuilder::addNeighbours(std::vector<std::set<int>>& neighbourhood, std::pair<int, int>&& neighbours) {
    neighbourhood[neighbours.first].insert(neighbours.second);
    neighbourhood[neighbours.second].insert(neighbours.first);
}
