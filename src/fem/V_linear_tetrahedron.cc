#include "V_linear_tetrahedron.h"
#include "../geometry.h"
#include "dphi_linear_tetrahedron_dX.h"
#include "psi_neo_hookean.h"

namespace nm::fem {
    auto singlePointQuadrature(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                               real weight) -> real {
        // Obtain the deformed space vertex position matrix for this element
        // x
        mat34r deformed;
        for (int ii = 0; ii < 4; ++ii) { deformed.col(ii) = q.segment<3>(element(ii) * 3); }

        // Get the centroid of the deformed coordinates
        const vec3r centroid = computeTetrahedralCentroid(element);

        // Obtain the deformation gradient for this element dx/dX
        // Obtain dphi/dX, the right hand side of the expression
        const mat43r dphi_dX = dphiLinearTetrahedronDx(vertices, element, centroid);

        // Obtain the deformation gradient
        const mat3r F = deformed * dphi_dX;

        // Compute psi with the deformation gradient.
        const real strain_energy_density = psiNeoHookean(F, mu, lambda);

        // Now, finish up by multiplying this value by the weight.
        return strain_energy_density * weight;
    }

    auto VlinearTetrahedron(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                            real volume) -> real {
        // Compute the potential energy via the quadrature value
        return singlePointQuadrature(q, vertices, element, mu, lambda, volume);
    }
}// namespace nm::fem
