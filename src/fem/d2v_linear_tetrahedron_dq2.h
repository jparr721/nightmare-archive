#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * Computes the hessian of the potential energy with respect to the reference coordinates, q. Since we're taking
     * the integral of the partial derivative, we can use quadrature in much the same way to compute that value by
     * relying on the hessian of the shape functions and the strain energy density function.
     * @param q The reference space coordinate values
     * @param vertices The vertices of all points in the mesh.
     * @param element The tetrahedral element we're targeting
     * @param mu Lame's mu parameter
     * @param lambda Lame's lambda parameter
     * @param volume The volume of the tetrahedral element we're targeting.
     * @return The 12x12 hessian matrix.
     */
    auto d2VlinearTetrahedronDq2(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                                 real volume) -> mat1212r;
}// namespace nm::fem