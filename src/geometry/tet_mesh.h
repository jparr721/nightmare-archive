#pragma once

#include "../math.h"
#include <map>
#include <string>
#include <vector>

namespace nm::geometry {

    class TetMesh {
#ifdef NDEBUG
        static constexpr const char *kDefaultTetgenFlags = "zpqQ";
#else
        static constexpr const char *kDefaultTetgenFlags = "zpq";
#endif

        enum TetgenResult {
            kTetgenFailedToConvert = -1,
            kTetgenCrashed = 1,
            kTetgenFailedToCreateTets = 2,
            kTetgenFailedToConvertToMatrix = 3,
        };

    public:
        TetMesh() = default;
        explicit TetMesh(const std::string &meshPath);
        TetMesh(const mat &vertices, const mati &faces);
        TetMesh(const mat &vertices, const mati &tetrahedra, const mati &faces);

        auto vertices() -> mat & { return vertices_; }
        auto vertices() const -> const mat & { return vertices_; }
        auto tetrahedra() -> mati & { return tetrahedra_; }
        auto tetrahedra() const -> const mati & { return tetrahedra_; }
        auto faces() -> mati & { return faces_; }
        auto faces() const -> const mati & { return faces_; }
        auto dofs() const -> int { return vertices_.rows() * 3; }

        void setVertices(const mat &vertices);
        void setTetrahedra(const mati &tetrahedra);
        void setFaces(const mati &faces);

        void translate(const vec3 &translation);
        void rotate(real angle, const vec3 &axis);

        /**
         * Begin tetrahedralization routine
         */
        void tetrahedralizeMesh();

    private:
        mat vertices_;
        mati tetrahedra_;
        mati faces_;

        std::vector<vec3i> surfaceTriangles_;
        std::vector<u32> surfaceVertices_;
        std::vector<vec2i> surfaceEdges_;

        std::map<int, int> volumeToSurfaceID_;

        void computeSurfaceTriangles();
        void computeSurfaceVertices();
        void computeSurfaceEdges();
    };
}// namespace nm::geometry
