#include "mesh.h"
#include <igl/read_triangle_mesh.h>

#include <utility>

namespace nm {
    Mesh::Mesh(const std::string &mesh_file) {
        igl::read_triangle_mesh(mesh_file, vertices, faces);
    }
}// namespace nm