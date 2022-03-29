#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * \brief Computes the mapping between reference space coordinates into the deformed space coordinates. For this, we
     * need to construct a linear system where we know A and b, but not x. In this case, we have something like:
     *
     * b = [X Y Z] (World Space Coordinates)\n
     * A = [X0 X1 X2 X3] (Matrix of X0, Y0, Z0, ... triplets)\n
     * x = Unknown (represents our matrix of shape functions for each point in our tetrahedral.\n
     *
     * We can then stack these together and solve the linear system to get the basis functions out.
     *
     * @param vertices The matrix of vertex positions in our entire simulated object.
     * @param tet The specific tetrahedral elements we are interested in accessing.
     * @param ref The reference space coordinates on which to compute.
     * @param phi The 4x1 values of the basis functions for phi.
     */
    void phi_linear_tetrahedron(const matXr &vertices, const vec4i &tet, const vec3r &ref, vec4r &phi);
}