#include "dV_linear_tetrahedron_dq.h"
#include "dphi_linear_tetrahedron_dX.h"
#include "dpsi_neo_hookean_dF.h"
#include "quadrature_single_point.h"

namespace nm::fem {
    auto dVlinearTetrahedronDq(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                               real volume) -> vec12r {
        const auto computeNeohookeanLinearTet = [&](const vecXr &deformedVertices, const vecXi &tetrahedralIndices,
                                                    const vec3r &centroid) -> vec12r {
            mat34r deformedTetrahedron;
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.col(ii) = q.segment<3>(3 * tetrahedralIndices(ii)); }

            // Obtain the shape function gradient matrix D;
            const auto D = dphiLinearTetrahedronDx(vertices, element, centroid);

            // Obtain the deformation gradient
            const mat3r F = deformedTetrahedron * D;

            // Compute the gradient of the strain energy density with respect to F.
            const vec9r dpsi = dpsiNeoHookeanDf(F, mu, lambda);

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