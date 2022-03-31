#include "geometry.h"


namespace nm {
    auto computeTetrahedralCentroid(const mat43r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.colwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralCentroid(const mat34r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.rowwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralElementVolume(const mat43r &tet) -> real {
        mat4r sq_tet = mat4r::Ones();
        sq_tet.block<4, 3>(0, 1) = tet;
        return sq_tet.determinant() / 6;
    }
}// namespace nm