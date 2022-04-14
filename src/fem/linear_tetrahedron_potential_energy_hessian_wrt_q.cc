#include "linear_tetrahedron_potential_energy_hessian_wrt_q.h"
#include "../geometry.h"
#include "linear_tetrahedron_basis_function_gradient_matrix.h"
#include "quadrature_single_point.h"
#include "strain_energy_density_neo_hookean_hessian_wrt_F.h"

namespace nm::fem {
    auto linearTetrahedronPotentialEnergyHessianWrtq(const vecXr &q, const matXr &vertices, const vec4i &element,
                                                     real mu, real lambda, real volume) -> mat1212r {
        const auto computeNeohookeanLinearTetHessian =
                [&](const vecXr &deformedVertices, const vecXi &tetrahedralIndices, const vec3r &centroid) -> mat1212r {
            mat34r deformedTetrahedron;
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.col(ii) = q.segment<3>(3 * tetrahedralIndices(ii)); }

            // Obtain the shape function gradient matrix D;
            const auto D = linearTetrahedronBasisFunctionGradientMatrix(vertices, element, centroid);

            // Obtain the deformation gradient
            const mat3r F = deformedTetrahedron * D;

            // Compute the hessian of the strain energy density with respect to F.
            const mat99r dpsi = strainEnergyDensityNeoHookeanHessianWrtF(F, mu, lambda);

            // Compute the matrix B from the shape function gradient matrix.
            mat912r B;
            B.setZero();
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) {
                B.block(0, 0 + 3 * ii, 3, 1) = D.row(ii).transpose();
                B.block(3, 1 + 3 * ii, 3, 1) = D.row(ii).transpose();
                B.block(6, 2 + 3 * ii, 3, 1) = D.row(ii).transpose();
            }

            return B.transpose() * dpsi * B;
        };

        mat1212r H;
        quadratureSinglePoint(computeNeohookeanLinearTetHessian, q, element, volume, H);

        // Make sure that the hessian is symmetric and positive definite.
        // We do this by projecting all of the negative eigenvalues to small, positive values.
        Eigen::SelfAdjointEigenSolver<mat1212r> eigenSolver(H);
        const matXr eigenVectors = eigenSolver.eigenvectors().real();
        matXr eigenvaluesDiagonal = eigenSolver.eigenvalues().real().asDiagonal();

#pragma unroll
        for (int ii = 0; ii < 12; ++ii) {
            if (eigenSolver.eigenvalues()[ii] < 1e-6) { eigenvaluesDiagonal(ii, ii) = 1e-3; }
        }

        H = eigenVectors * eigenvaluesDiagonal * eigenVectors.transpose();
        return H;
    }
}// namespace nm::fem