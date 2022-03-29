#include "phi_linear_tetrahedron.h"

namespace nm::fem {
    void phi_linear_tetrahedron(const nm::matXr &vertices, const vec4i &tet, const nm::vec3r &ref, nm::vec4r &phi) {
        const auto X0 = vertices.row(tet(0));
        const auto X1 = vertices.row(tet(1));
        const auto X2 = vertices.row(tet(2));
        const auto X3 = vertices.row(tet(3));

        // The matrix T represents the edge vectors from our source point X0 to each of the other X points in reference
        // space
        mat3r T;
        T.col(0) = X1 - X0;
        T.col(1) = X2 - X0;
        T.col(2) = X3 - X0;

        // Now, we must apply our reference coordinate difference to get the other side of the matrix function.
        vec3r b = ref - X0;

        // Now, we can solve the linear system. As Ax=b where A is T and phi is our basis function mapping.
        phi.segment<3>(1) = T.fullPivHouseholderQr().solve(b); // phi_1, 2, 3

        // phi_0 is a linear combination of its elements.
        phi(0) = 1 - phi(1) - phi(2) - phi(3);
    }

}// namespace nm::fem