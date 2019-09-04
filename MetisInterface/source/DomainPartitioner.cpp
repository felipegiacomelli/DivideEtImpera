#include "DivideEtImpera/MetisInterface/DomainPartitioner.hpp"
#include <metis.h>

std::vector<int> DomainPartitioner::partitionDomain(GridDataPtr gridData, int numberOfPartitions) {
    int options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_CTYPE] = METIS_CTYPE_SHEM;
    options[METIS_OPTION_RTYPE] = METIS_RTYPE_GREEDY;
    options[METIS_OPTION_NUMBERING] = 0;

    int nodes = gridData->coordinates.size();
    int constrains = 1;
    int parts = numberOfPartitions;
    int objval;
    std::vector<int> subdomains(gridData->coordinates.size());

    this->buildMetisVectors(gridData);

    METIS_PartGraphKway(&nodes, &constrains, &this->xadj[0], &this->adjncy[0], nullptr, nullptr, nullptr, &parts, nullptr, nullptr, options, &objval, &subdomains[0]);

    return subdomains;
}

void DomainPartitioner::buildMetisVectors(GridDataPtr gridData) {
    this->xadj.reserve(gridData->coordinates.size() + 1);
    std::vector<std::set<int>> neighbourhood(this->graphBuilder.buildGraph(gridData));
    this->adjncy.reserve(std::accumulate(neighbourhood.cbegin(), neighbourhood.cend(), 0, [](int s, const auto& n){return s + n.size();}));

    this->xadj.emplace_back(0);
    for (const auto& neighbours : neighbourhood) {
        this->xadj.emplace_back(this->xadj.back() + neighbours.size());
        for (auto neighbour : neighbours) {
            this->adjncy.emplace_back(neighbour);
        }
    }
}
