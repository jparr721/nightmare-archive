#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * We compute the integral of the tetrahedral domain which, in this case, is just the density * volume / 10.
     * This is typically computed with a symbolic package, but the integral is the following:
     * \n
     * 6p * vol * integral(
     *   integral(
     *      integral(phi_1^2, (phi_3, 0, 1 - phi_1 - phi_2)),
     *   (phi_2, 0, 1 - phi_1)),
     * (phi_1, 0, 1))
     *
     * @param density Our known density value
     * @param volume The tetrahedral volume (det(V) / 6)
     * @return The 12x12 mass matrix.
     */
    auto massMatrixLinearTetrahedron(real density, real volume) -> mat1212r;

    /**
     * Computes the kinetic energy of the linear tetrahedron according to the following formula:\n
     * T = 1/2 qot^T * M_e * qdot
     *
     * @param qdot The velocity of the deformed space coordinates.
     * @param element The tetrahedral element we are computing on.
     * @param density The density of the tetrahedral element
     * @param volume The volume of the tetrahedral element.
     * @return The kinetic energy of the linear tetrahedron.
     */
    auto TlinearTetrahedron(const vecXr &qdot, const vecXi &element, real density, real volume) -> real;

}// namespace nm::fem