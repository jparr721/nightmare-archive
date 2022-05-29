#include "square.h"

namespace nm::geometry::primitives {
    auto loadCubeFaces() -> mati {
        mati F;
        F.resize(12, 3);
        F.row(0) = vec3i(4, 7, 6);
        F.row(1) = vec3i(4, 6, 5);
        F.row(2) = vec3i(3, 0, 1);
        F.row(3) = vec3i(3, 1, 2);
        F.row(4) = vec3i(7, 3, 2);
        F.row(5) = vec3i(7, 2, 6);
        F.row(6) = vec3i(0, 4, 5);
        F.row(7) = vec3i(0, 5, 1);
        F.row(8) = vec3i(2, 1, 5);
        F.row(9) = vec3i(2, 5, 6);
        F.row(10) = vec3i(0, 3, 7);
        F.row(11) = vec3i(0, 7, 4);
    }

    auto loadCubeVertices() -> mat {
        mat V;
        V.resize(8, 3);
        V.row(0) = vec3(0.0, 0.0, 0.0);
        V.row(1) = vec3(0.0, 1.0, 0.0);
        V.row(2) = vec3(1.0, 1.0, 0.0);
        V.row(3) = vec3(1.0, 0.0, 0.0);
        V.row(4) = vec3(0.0, 0.0, 1.0);
        V.row(5) = vec3(0.0, 1.0, 1.0);
        V.row(6) = vec3(1.0, 1.0, 1.0);
        V.row(7) = vec3(1.0, 0.0, 1.0);
    }

    void loadSquareGeometry(mat &V, mati &F) {
        F = loadCubeFaces();
        V = loadCubeVertices();
    }
}// namespace nm::geometry::primitives