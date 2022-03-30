#include "geometry.h"


namespace nm {
    auto computeTetrahedralCentroid(const mat43r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.colwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralCentroid(const mat34r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.rowwise().sum().array() / 4).matrix();
    }
}// namespace nm