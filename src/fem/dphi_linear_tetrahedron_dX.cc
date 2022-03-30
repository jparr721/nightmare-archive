#include "dphi_linear_tetrahedron_dX.h"
namespace nm::fem {
    auto dphiLinearTetrahedronDx(const matXr &vertices, const vec4i &element, const vec3r &ref) -> mat43r {
        // Get our reference space coordinates
        const vec3r X0 = vertices.row(element(0));
        const vec3r X1 = vertices.row(element(1));
        const vec3r X2 = vertices.row(element(2));
        const vec3r X3 = vertices.row(element(3));

        // Construct T as the matrix of deltas
        mat3r T;
        T.col(0) = X1 - X0;
        T.col(1) = X2 - X0;
        T.col(2) = X3 - X0;

        // Construct the 4x3 matrix of -1^T * TT and TT.
        mat3r TT = T.inverse();
        vec3r top_row = -vec3r::Ones().transpose() * TT;
        mat43r dphi;
        dphi.row(0) = top_row;
        dphi.block<3, 3>(1, 0) = TT;

        return dphi;
    }
}// namespace nm::fem
