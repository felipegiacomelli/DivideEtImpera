 #ifndef __BOOST_INTERFACE_SERIALIZATION_HPP__
 #define __BOOST_INTERFACE_SERIALIZATION_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <MSHtoCGNS/GridData/GridData.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive& ar, SectionData& sectionData, const unsigned int) {
            ar & sectionData.name;
            ar & sectionData.dimension;
            ar & sectionData.begin;
            ar & sectionData.end;
            ar & sectionData.vertices;
        }

        template<class Archive>
        void serialize(Archive& ar, GridData& gridData, const unsigned int) {
            ar & gridData.dimension;

            ar & gridData.coordinates;
            ar & gridData.connectivities;
            ar & gridData.sections;

            ar & gridData.numberOfLocalVertices;
        }
    }
}

#endif
