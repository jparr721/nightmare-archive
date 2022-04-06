#include "dV_linear_tetrahedron_dq.h"
#include "../geometry.h"
#include "dphi_linear_tetrahedron_dX.h"
#include "dpsi_neo_hookean_dF.h"

namespace nm::fem {
    auto dVlinearTetrahedronDq(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                               real volume) -> vec12r {
        // Obtain the deformed space vertex position matrix for this element
        mat34r deformed;
        for (int ii = 0; ii < 4; ++ii) { deformed.col(ii) = q.segment<3>(element(ii) * 3); }

        // Get the centroid of the deformed coordinates
        const vec3r centroid = computeTetrahedralCentroid(deformed);

        // Obtain the shape function gradient matrix, D.
        const mat43r dphiDX = dphiLinearTetrahedronDx(vertices, element, centroid);

        // Obtain the deformation gradient
        const mat3r F = deformed * dphiDX;

        // Compute the gradient of the strain energy density with respect to F.
        const vec9r dpsi = dpsiNeoHookeanDf(F, mu, lambda);

        // Now, construct B_j from the shape function gradient matrix.
        mat912r B;
        for (int ii = 0; ii < 4; ++ii) {
            B.block(0, 0 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
            B.block(3, 1 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
            B.block(6, 2 + 3 * ii, 3, 1) = dphiDX.row(ii).transpose();
        }

        return -volume * (B.transpose() * dpsi);
    }

}// namespace nm::fem