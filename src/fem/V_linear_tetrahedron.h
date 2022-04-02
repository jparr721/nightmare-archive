#pragma once

#include "../nm_math.h"
namespace nm::fem {

    /**
     * Computes the potential energy integral from the strain energy density function
     * @param q The deformed space coordinate vector.
     * @param vertices The vertices of all tetrahedral elements.
     * @param element The coordinate vector of our elements.
     * @param mu The lame's mu parameter.
     * @param lambda The lame's lambda parameter.
     * @param volume The volume of the tetrahedral.
     * @return The potential energy.
     */
    auto VlinearTetrahedron(const vecXr &q, const matXr &vertices, const vec4i &element, real mu, real lambda,
                            real volume) -> real;
}// namespace nm::fem