#include "linear_tetrahedron_potential_energy_gradient_wrt_q.h"
#include "linear_tetrahedron_basis_function_gradient_matrix.h"
#include "quadrature_single_point.h"
#include "strain_energy_density_neo_hookean_gradient_wrt_F.h"

namespace nm::fem {
    auto linearTetrahedronPotentialEnergyGradientWrtq(const vecXr &q, const matXr &vertices, const vec4i &element,
                                                      real mu, real lambda, real volume) -> vec12r {
        const auto computeNeohookeanLinearTet = [&](const vecXr &deformedVertices, const vecXi &tetrahedralIndices,
                                                    const vec3r &centroid) -> vec12r {
            mat34r deformedTetrahedron;
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.col(ii) = q.segment<3>(3 * tetrahedralIndices(ii)); }

            // Obtain the shape function gradient matrix D;
            const auto D = linearTetrahedronBasisFunctionGradientMatrix(vertices, element, centroid);

            // Obtain the deformation gradient
            const mat3r F = deformedTetrahedron * D;

            // Compute the gradient of the strain energy density with respect to F.
            const vec9r dpsi = strainEnergyDensityNeoHookeanGradientWrtF(F, mu, lambda);

            // Compute the matrix B from the shape function gradient matrix.
            mat912r B;
            B.setZero();
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) {
                B.block(0, 0 + 3 * ii, 3, 1) = D.row(ii).transpose();
                B.block(3, 1 + 3 * ii, 3, 1) = D.row(ii).transpose();
                B.block(6, 2 + 3 * ii, 3, 1) = D.row(ii).transpose();
            }

            return B.transpose() * dpsi;
        };

        vec12r potentialEnergyGradient;
        quadratureSinglePoint(computeNeohookeanLinearTet, q, element, volume, potentialEnergyGradient);
        return potentialEnergyGradient;
    }

}// namespace nm::fem