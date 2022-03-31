#include "T_linear_tetrahedron.h"
#include "mass_matrix_linear_tetrahedron.h"

namespace nm::fem {
    auto TlinearTetrahedron(const vecXr &qdot, const vecXi &element, real density, real volume) -> real {
        const mat1212r M = massMatrixLinearTetrahedron(density, volume);

        // Index into qdot to find the elements that we want.
        vec12r v;
        v.segment<3>(0) = qdot.segment<3>(element(0) * 3);
        v.segment<3>(3) = qdot.segment<3>(element(1) * 3);
        v.segment<3>(6) = qdot.segment<3>(element(2) * 3);
        v.segment<3>(9) = qdot.segment<3>(element(3) * 3);
        return 0.5 * qdot.transpose() * M * qdot;
    }
}// namespace nm::fem
