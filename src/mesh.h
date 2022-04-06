#pragma once

#include "nm_math.h"
#include <string>
#include <utility>

namespace nm {
#ifdef NDEBUG
    constexpr const char *kDefaultTetgenFlags = "zpqQa1";
#else
    constexpr const char *kDefaultTetgenFlags = "zpq";
#endif

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
    auto tetrahedralizeMesh(Mesh *mesh, const char *flags = kDefaultTetgenFlags) -> bool;
}// namespace nm