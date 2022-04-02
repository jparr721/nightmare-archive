#include "geometry.h"
#include <spdlog/spdlog.h>


namespace nm {
    auto computeTetrahedralCentroid(const mat43r &tetrahedralElement) -> vec3r {
        return (tetrahedralElement.colwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralCentroid(const mat34r &tetrahedralElement) -> vec3r {
        return (tetrahedralElement.rowwise().sum().array() / 4).matrix();
    }

    auto computeTetrahedralElementVolume(const mat43r &tet) -> real {
        mat4r sqTet = mat4r::Ones();
        sqTet.block<4, 3>(0, 1) = tet;
        return sqTet.determinant() / 6;
    }

    auto findMinVertices(const matXr &vertices, real tol) -> std::vector<int> {
        std::vector<int> minVertices;
        real minVertex = vertices.row(0)(1);

        for (int ii = 0; ii < vertices.rows(); ++ii) { minVertex = std::min(minVertex, vertices.row(ii)(1)); }

        // Compute the min vertices
        for (int ii = 0; ii < vertices.rows(); ++ii) {
            if (std::abs(vertices.row(ii)(1) - minVertex) < tol) { minVertices.push_back(ii); }
        }

        if (minVertices.size() < vertices.rows() * 0.03) {
            spdlog::warn("Min vertices make up less than 3% of the mesh, there might be missing vertices");
        }

        return minVertices;
    }
}// namespace nm