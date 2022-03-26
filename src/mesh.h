#pragma once

#include "nm_math.h"
#include <string>

namespace nm {
    struct Mesh {
        rowMatXr vertices;
        rowMatXi faces;
        rowMatXi tetrahedra;
        rowMatXr normals;
        rowMatXr colors;

        Mesh() = default;
        explicit Mesh(const std::string &mesh_file);
//        Mesh(vecXr vertices, vecXr colors);
//        Mesh(vecXr vertices, vecXr colors, vecXr normals = vecXr());
    };
}// namespace nm