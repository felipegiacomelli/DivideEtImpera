 #ifndef __BOOST_INTERFACE_SERIALIZATION_HPP__
 #define __BOOST_INTERFACE_SERIALIZATION_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <MSHtoCGNS/GridData/GridData.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive& ar, EntityData& entityData, const unsigned int) {
            ar & entityData.name;
            ar & entityData.begin;
            ar & entityData.end;
            ar & entityData.vertices;
        }

        template<class Archive>
        void serialize(Archive& ar, GridData& gridData, const unsigned int) {
            ar & gridData.dimension;

            ar & gridData.coordinates;

            ar & gridData.tetrahedrons;
            ar & gridData.hexahedrons;
            ar & gridData.prisms;
            ar & gridData.pyramids;
            ar & gridData.triangles;
            ar & gridData.quadrangles;
            ar & gridData.lines;
            ar & gridData.points;

            ar & gridData.boundaries;
            ar & gridData.regions;
            ar & gridData.wells;

            ar & gridData.numberOfLocalVertices;
        }
    }
}

#endif
