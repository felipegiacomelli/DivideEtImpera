#include <MSHtoCGNS/CgnsInterface/CgnsWriter.hpp>
#include <MSHtoCGNS/CgnsInterface/CgnsReader.hpp>
#include "DivideEtImpera/DomainDivider/DomainDivider.hpp"
#include "DivideEtImpera/DomainDivider/DomainCreator.hpp"
#include "DivideEtImpera/DomainDivider/SubdomainCreator.hpp"

int main() {
    boost::mpi::environment environment;
    boost::mpi::communicator world;

    if (world.size() == 0)
        return 0;

    std::string input{"/home/felipe/Downloads/tcc/19v_24e/Grid.cgns"};
    std::string output{"/home/felipe/Downloads"};

    GridDataPtr globalGridData;
    if (world.rank() == 0) {
        CgnsReader cgnsReader(input);
        globalGridData = cgnsReader.gridData;
    }

    // if (world.rank() == 0) {
    //     GridDataPtr gridData;
    //     std::string file("/home/felipe/Downloads/tcc/19v_24e/Grid.cgns");
    //     {
    //         CgnsReader cgnsReader(file);
    //         gridData = cgnsReader.gridData;
    //     }

    //     DomainPartitioner domainPartitioner;
    //     auto subdomain{domainPartitioner.partitionDomain(gridData, 2)}; // METIS_PartGraphRecursive

    //     printf("\n\t0: %li", std::count(subdomain.begin(), subdomain.end(), 0));
    //     printf("\n\t1: %li", std::count(subdomain.begin(), subdomain.end(), 1));

    //     CgnsWriter cgnsWriter(file, "Vertex");
    //     cgnsWriter.writeSolution("Solution");
    //     cgnsWriter.writeField("Subdomains", subdomain);
    // }
    if (world.rank() == 0) {
        DomainDivider domainDivider;
        SubdomainDataPtr subdomainData = domainDivider.divideDomain(globalGridData);

        {
            SubdomainCreator subdomainCreator(subdomainData->localGridData, "/home/felipe/Downloads/tcc/19v_24e/", false);
            std::vector<int> subdomain(subdomainData->localGridData->coordinates.size(), world.rank());
            for (unsigned i = subdomainData->localGridData->numberOfLocalVertices; i < subdomain.size(); ++i) subdomain[i] = 1;
            CgnsWriter cgnsWriter(subdomainCreator.getFileName(), "Vertex");
            cgnsWriter.writeSolution("Solution");
            cgnsWriter.writeField("Subdomains", subdomain);
            cgnsWriter.writeField("GlobalIndices", subdomainData->globalIndices);
        }

        {
            DomainCreator domainCreator(globalGridData, subdomainData, "/home/felipe/Downloads/tcc/19v_24e/new.cgns");
            std::vector<int> subdomain(globalGridData->coordinates.size(), 1);
            for (int i = 0; i < subdomainData->localGridData->numberOfLocalVertices; ++i) subdomain[i] = 0;
            CgnsWriter cgnsWriter(domainCreator.getFileName(), "Vertex");
            cgnsWriter.writeSolution("Solution");
            cgnsWriter.writeField("Subdomains", subdomain);
        }
    }
    else {
        DomainDivider domainDivider;
        SubdomainDataPtr subdomainData = domainDivider.divideDomain();

        {
            SubdomainCreator subdomainCreator(subdomainData->localGridData, "/home/felipe/Downloads/tcc/19v_24e/", false);
            std::vector<int> subdomain(subdomainData->localGridData->coordinates.size(), world.rank());
            for (unsigned i = subdomainData->localGridData->numberOfLocalVertices; i < subdomain.size(); ++i) subdomain[i] = 0;
            CgnsWriter cgnsWriter(subdomainCreator.getFileName(), "Vertex");
            cgnsWriter.writeSolution("Solution");
            cgnsWriter.writeField("Subdomains", subdomain);
            cgnsWriter.writeField("GlobalIndices", subdomainData->globalIndices);
        }
    }

    return 0;
}
