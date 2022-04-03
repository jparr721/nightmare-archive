#pragma once

#include "nm_math.h"
#include <string>
#include <utility>

namespace nm {
    constexpr const char *kDefaultTetgenFlags = "zpqQa1e-1";

    enum TetgenResult {
        kTetgenFailedToConvert = -1,
        kTetgenCrashed = 1,
        kTetgenFailedToCreateTets = 2,
        kTetgenFailedToConvertToMatrix = 3,
    };

    struct Mesh {
        int id = 0;
        matXr vertices;
        matXi faces;
        matXi tetrahedra;

        Mesh() = default;
        explicit Mesh(const std::string &meshFile);
        Mesh(matXr vertices, matXi faces) : vertices(std::move(vertices)), faces(std::move(faces)) {}
    };

    void translateMesh(Mesh *mesh, const vec3r &amount);
    void tetrahedralizeMesh(Mesh *mesh, const char *flags = kDefaultTetgenFlags);
}// namespace nm