#include "geometry.h"
#include <spdlog/spdlog.h>


namespace nm {
    auto computeTetrahedralCentroid(const mat43r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.colwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralCentroid(const mat34r &tetrahedral_element) -> vec3r {
        return (tetrahedral_element.rowwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralElementVolume(const mat43r &tet) -> real {
        mat4r sq_tet = mat4r::Ones();
        sq_tet.block<4, 3>(0, 1) = tet;
        return sq_tet.determinant() / 6;
    }

    auto findMinVertices(const matXr &vertices, real tol) -> std::vector<int> {
        std::vector<int> min_vertices;
        real min_vertex = vertices.row(0)(1);

        for (int ii = 0; ii < vertices.rows(); ++ii) { min_vertex = std::min(min_vertex, vertices.row(ii)(1)); }

        // Compute the min vertices
        for (int ii = 0; ii < vertices.rows(); ++ii) {
            if (std::abs(vertices.row(ii)(1) - min_vertex) < tol) { min_vertices.push_back(ii); }
        }

        if (min_vertices.size() < vertices.rows() * 0.03) {
            spdlog::warn("Min vertices make up less than 3% of the mesh, there might be missing vertices");
        }

        return min_vertices;
    }
}// namespace nm