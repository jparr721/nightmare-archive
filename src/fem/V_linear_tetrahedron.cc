#include "V_linear_tetrahedron.h"
#include "dphi_linear_tetrahedron_dX.h"
#include "psi_neo_hookean.h"
#include "quadrature_single_point.h"

namespace nm::fem {
    auto VlinearTetrahedron(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                            real volume) -> real {
        const auto computeNeohookeanPotentialEnergy = [&](const vecXr &deformedVertices,
                                                          const vecXi &tetrahedralIndices, const vec3r &centroid) {
            mat34r deformedTetrahedron;
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.col(ii) = q.segment<3>(3 * tetrahedralIndices(ii)); }

            // Obtain the shape function gradient matrix D;
            const auto D = dphiLinearTetrahedronDx(vertices, element, centroid);

            // Obtain the deformation gradient
            const mat3r F = deformedTetrahedron * D;

            // Compute the strain energy density for the neohookean model
            return psiNeoHookean(F, mu, lambda);
        };

        real potentialEnergy;
        quadratureSinglePoint(computeNeohookeanPotentialEnergy, q, element, volume, potentialEnergy);
        return potentialEnergy;
    }
}// namespace nm::fem
