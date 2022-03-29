#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * Computes the piecewise constant gradient matrix for the linear space functions. Each row contains the
     * gradient of the ith shape function computed from the phi_linear_tetrahedron code.
     *\n
     * Similar to the previous version, we use our matrix T to express these properties. So, we have to formulate
     * the new matrix as:\n
     * \[ -1^T * T^-1]\n
     * \[T^-1]\n
     *\n
     * Where this matrix is in R^4x3 where the top row is a vector from the matrix-vector application, and the rest is
     * just T inverse.
     *
     * @param vertices The vertex positions of the entire simulated object.
     * @param tet The tetrahedral vertex indices for this element.
     * @param ref The reference space tetrahedral where we're computing the energy density.
     * @param dphi The 4x3 gradient basis functions with respect to X. dPhi / dX.
     */
    void dphiLinearTetrahedronDx(const matXr &vertices, const vec4i &tet, const vec3r &ref, mat43r &dphi);
}// namespace nm::fem