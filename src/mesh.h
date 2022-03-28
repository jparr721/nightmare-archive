#pragma once

#include "nm_math.h"
#include <string>
#include <utility>

namespace nm {
    struct Mesh {
        matXr vertices;
        matXi faces;
        matXi tetrahedra;

        Mesh() = default;
        explicit Mesh(const std::string &mesh_file);
        Mesh(matXr vertices, matXi faces) : vertices(std::move(vertices)), faces(std::move(faces)) {}
    };
}// namespace nm