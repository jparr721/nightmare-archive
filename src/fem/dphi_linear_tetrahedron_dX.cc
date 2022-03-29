#include "dphi_linear_tetrahedron_dX.h"
namespace nm::fem {
    void dphiLinearTetrahedronDx(const nm::matXr &vertices, const nm::vec4i &tet, const nm::vec3r &ref,
                                 nm::mat43r &dphi) {
        // Get our reference space coordinates
        const vec3r X0 = vertices.row(tet(0));
        const vec3r X1 = vertices.row(tet(1));
        const vec3r X2 = vertices.row(tet(2));
        const vec3r X3 = vertices.row(tet(3));

        // Construct T as the matrix of deltas
        mat3r T;
        T.col(0) = X1 - X0;
        T.col(1) = X2 - X0;
        T.col(2) = X3 - X0;

        mat3r TT = T.inverse();
        vec3r top_row = -vec3r::Ones().transpose() * TT;
        dphi.row(0) = top_row;
        dphi.block<3, 3>(1, 0) = TT;
    }
}// namespace nm::fem
