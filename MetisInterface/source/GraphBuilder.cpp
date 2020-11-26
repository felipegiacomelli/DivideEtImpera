#include "DivideEtImpera/MetisInterface/GraphBuilder.hpp"
#include <cgnslib.h>

std::vector<std::set<int>> GraphBuilder::buildGraph(GridDataPtr gridData) {
    std::vector<std::set<int>> neighbourhood(gridData->coordinates.size());
    for (auto section : gridData->sections) {
        if (section.dimension == gridData->dimension) {
            for (auto element = gridData->connectivities.cbegin() + section.begin; element != gridData->connectivities.cbegin() + section.end; ++element) {
                switch (element->front()) {
                    case TETRA_4:
                        this->addNeighbours(neighbourhood, this->getTetrahedronFaceNeighbours(element));
                        break;
                    case TETRA_10:
                        this->addNeighbours(neighbourhood, this->getTetrahedronFaceNeighbours(element));
                        break;
                    case TETRA_16:
                        this->addNeighbours(neighbourhood, this->getTetrahedronFaceNeighbours(element));
                        break;
                    case TETRA_20:
                        this->addNeighbours(neighbourhood, this->getTetrahedronFaceNeighbours(element));
                        break;

                    case HEXA_8:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;
                    case HEXA_20:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;
                    case HEXA_27:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;
                    case HEXA_32:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;
                    case HEXA_56:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;
                    case HEXA_64:
                        this->addNeighbours(neighbourhood, this->getHexahedronFaceNeighbours(element));
                        break;

                    case PENTA_6:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;
                    case PENTA_15:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;
                    case PENTA_18:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;
                    case PENTA_24:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;
                    case PENTA_38:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;
                    case PENTA_40:
                        this->addNeighbours(neighbourhood, this->getPrismFaceNeighbours(element));
                        break;

                    case PYRA_5:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;
                    case PYRA_14:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;
                    case PYRA_13:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;
                    case PYRA_21:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;
                    case PYRA_29:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;
                    case PYRA_30:
                        this->addNeighbours(neighbourhood, this->getPyramidFaceNeighbours(element));
                        break;

                    case TRI_3:
                        this->addNeighbours(neighbourhood, this->getTriangleFaceNeighbours(element));
                        break;
                    case TRI_6:
                        this->addNeighbours(neighbourhood, this->getTriangleFaceNeighbours(element));
                        break;
                    case TRI_9:
                        this->addNeighbours(neighbourhood, this->getTriangleFaceNeighbours(element));
                        break;
                    case TRI_10:
                        this->addNeighbours(neighbourhood, this->getTriangleFaceNeighbours(element));
                        break;

                    case QUAD_4:
                        this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(element));
                        break;
                    case QUAD_8:
                        this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(element));
                        break;
                    case QUAD_9:
                        this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(element));
                        break;
                    case QUAD_12:
                        this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(element));
                        break;
                    case QUAD_16:
                        this->addNeighbours(neighbourhood, this->getQuadrangleFaceNeighbours(element));
                        break;

                    default:
                        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + " - Element type " + std::to_string(element->front()) + " not supported number");
                }
            }
        }
    }
    return neighbourhood;
}

std::vector<std::pair<int, int>> GraphBuilder::getTetrahedronFaceNeighbours(std::vector<std::vector<int>>::const_iterator tetrahedron) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*tetrahedron)[1], (*tetrahedron)[2]),
        std::make_pair((*tetrahedron)[2], (*tetrahedron)[3]),
        std::make_pair((*tetrahedron)[3], (*tetrahedron)[1]),
        std::make_pair((*tetrahedron)[1], (*tetrahedron)[4]),
        std::make_pair((*tetrahedron)[2], (*tetrahedron)[4]),
        std::make_pair((*tetrahedron)[3], (*tetrahedron)[4])
    };
}

std::vector<std::pair<int, int>> GraphBuilder::getHexahedronFaceNeighbours(std::vector<std::vector<int>>::const_iterator hexahedron) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*hexahedron)[1], (*hexahedron)[2]),
        std::make_pair((*hexahedron)[2], (*hexahedron)[3]),
        std::make_pair((*hexahedron)[3], (*hexahedron)[4]),
        std::make_pair((*hexahedron)[4], (*hexahedron)[1]),
        std::make_pair((*hexahedron)[5], (*hexahedron)[6]),
        std::make_pair((*hexahedron)[6], (*hexahedron)[7]),
        std::make_pair((*hexahedron)[7], (*hexahedron)[8]),
        std::make_pair((*hexahedron)[8], (*hexahedron)[5]),
        std::make_pair((*hexahedron)[5], (*hexahedron)[1]),
        std::make_pair((*hexahedron)[6], (*hexahedron)[2]),
        std::make_pair((*hexahedron)[7], (*hexahedron)[3]),
        std::make_pair((*hexahedron)[8], (*hexahedron)[4])
    };
}

std::vector<std::pair<int, int>> GraphBuilder::getPrismFaceNeighbours(std::vector<std::vector<int>>::const_iterator prism) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*prism)[1], (*prism)[2]),
        std::make_pair((*prism)[2], (*prism)[3]),
        std::make_pair((*prism)[3], (*prism)[1]),
        std::make_pair((*prism)[5], (*prism)[4]),
        std::make_pair((*prism)[6], (*prism)[5]),
        std::make_pair((*prism)[4], (*prism)[6]),
        std::make_pair((*prism)[4], (*prism)[1]),
        std::make_pair((*prism)[5], (*prism)[2]),
        std::make_pair((*prism)[6], (*prism)[3])
    };
}

std::vector<std::pair<int, int>> GraphBuilder::getPyramidFaceNeighbours(std::vector<std::vector<int>>::const_iterator pyramid) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*pyramid)[1], (*pyramid)[2]),
        std::make_pair((*pyramid)[2], (*pyramid)[3]),
        std::make_pair((*pyramid)[3], (*pyramid)[4]),
        std::make_pair((*pyramid)[4], (*pyramid)[1]),
        std::make_pair((*pyramid)[1], (*pyramid)[5]),
        std::make_pair((*pyramid)[2], (*pyramid)[5]),
        std::make_pair((*pyramid)[3], (*pyramid)[5]),
        std::make_pair((*pyramid)[4], (*pyramid)[5])
    };
}

std::vector<std::pair<int, int>> GraphBuilder::getTriangleFaceNeighbours(std::vector<std::vector<int>>::const_iterator triangle) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*triangle)[1], (*triangle)[2]),
        std::make_pair((*triangle)[2], (*triangle)[3]),
        std::make_pair((*triangle)[3], (*triangle)[1])
    };
}

std::vector<std::pair<int, int>> GraphBuilder::getQuadrangleFaceNeighbours(std::vector<std::vector<int>>::const_iterator quadrangle) {
    return std::vector<std::pair<int, int>>{
        std::make_pair((*quadrangle)[1], (*quadrangle)[2]),
        std::make_pair((*quadrangle)[2], (*quadrangle)[3]),
        std::make_pair((*quadrangle)[3], (*quadrangle)[4]),
        std::make_pair((*quadrangle)[4], (*quadrangle)[1])
    };
}

void GraphBuilder::addNeighbours(std::vector<std::set<int>>& neighbourhood, std::vector<std::pair<int, int>>&& neighbours) {
    for (const auto& neighbor : neighbours) {
        neighbourhood[neighbor.first].insert(neighbor.second);
        neighbourhood[neighbor.second].insert(neighbor.first);
    }
}
