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
    };

    void initializeMesh(const std::string &meshfile, Mesh &mesh);

    void translateMesh(Mesh &mesh, const vec3r &amount);
    auto tetrahedralizeMesh(Mesh &mesh, const char *flags = kDefaultTetgenFlags) -> bool;
}// namespace nm