#include "linear_tetrahedron_basis_function_gradient_matrix.h"
namespace nm::fem {
    auto linearTetrahedronBasisFunctionGradientMatrix(const matXr &vertices, const vec4i &element, const vec3r &ref)
            -> mat43r {
        // Get our reference space coordinates
        const vec3r X0 = vertices.row(element(0)).transpose();
        const vec3r X1 = vertices.row(element(1)).transpose();
        const vec3r X2 = vertices.row(element(2)).transpose();
        const vec3r X3 = vertices.row(element(3)).transpose();

        // Construct T as the matrix of deltas
        mat3r T;
        T.col(0) = X1 - X0;
        T.col(1) = X2 - X0;
        T.col(2) = X3 - X0;

        // Construct the 4x3 matrix of -1^T * TT and TT.
        mat3r Tinv = T.inverse();
        vec3r topRow = -vec3r::Ones().transpose() * Tinv;
        mat43r basisFunctionGradientMatrix;
        basisFunctionGradientMatrix.row(0) = topRow;
        basisFunctionGradientMatrix.block<3, 3>(1, 0) = Tinv;

        return basisFunctionGradientMatrix;
    }
}// namespace nm::fem
