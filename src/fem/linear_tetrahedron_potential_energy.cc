#include "linear_tetrahedron_potential_energy.h"
#include "linear_tetrahedron_basis_function_gradient_matrix.h"
#include "quadrature_single_point.h"
#include "strain_energy_density_neo_hookean.h"

namespace nm::fem {
    auto linearTetrahedronPotentialEnergy(const vecXr &q, const matXr &vertices, const vec4i &element, real mu,
                                          real lambda, real volume) -> real {
        const auto computeNeoHookeanPotentialEnergy = [&](const vecXr &deformedVertices,
                                                          const vecXi &tetrahedralIndices, const vec3r &centroid) {
            mat34r deformedTetrahedron;
#pragma unroll
            for (int ii = 0; ii < 4; ++ii) { deformedTetrahedron.col(ii) = q.segment<3>(3 * tetrahedralIndices(ii)); }

            // Obtain the shape function gradient matrix shapeFunctionGradientMatrix;
            const auto shapeFunctionGradientMatrix = linearTetrahedronBasisFunctionGradientMatrix(vertices, element);

            // Obtain the deformation gradient
            const mat3r deformationGradient = deformedTetrahedron * shapeFunctionGradientMatrix;

            // Compute the strain energy density for the neohookean model
            return strainEnergyDensityNeoHookean(deformationGradient, mu, lambda);
        };

        real potentialEnergy;
        quadratureSinglePoint(computeNeoHookeanPotentialEnergy, q, element, volume, potentialEnergy);
        return potentialEnergy;
    }
}// namespace nm::fem
