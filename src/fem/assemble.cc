#include "assemble.h"
#include "dV_linear_tetrahedron_dq.h"

namespace nm::fem {

    auto assembleForces(const vecXr &q, const matXr &vertices, const matXi &tets, const vecXr &tet_volumes, real mu,
                        real lambda) -> vecXr {
        vecXr f;
        f.resize(q.rows());
        f.setZero();

        // Compute the force per tet, which is just the gradient of the potential energy with respect to q.
        for (int ii = 0; ii < tets.rows(); ++ii) {
            // Obtain the tetrahedral element force with respect to the reference coordinates
            const vec4i element = tets.row(ii);
            const vec12r f_e = dVlinearTetrahedronDq(q, vertices, element, mu, lambda, tet_volumes(ii));

            // Place into the vector assembly.
            f.segment<3>(3 * tets(ii, 0)) += f_e.segment<3>(0);
            f.segment<3>(3 * tets(ii, 1)) += f_e.segment<3>(3);
            f.segment<3>(3 * tets(ii, 2)) += f_e.segment<3>(6);
            f.segment<3>(3 * tets(ii, 3)) += f_e.segment<3>(9);
        }

        return f;
    }
}
