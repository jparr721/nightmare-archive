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
        auto ndofs() const -> int { return vertices_.rows() * 3; }

        auto displacement() const -> vec;

        void setRayleighAlpha(real rayleighAlpha);
        void setRayleighBeta(real rayleighBeta);
        void setVertices(const mat &vertices);
        void setVerticesFromPositions(const vec &positions);
        void setDisplacement(const vec &delta);
        void addDisplacement(const vec &delta);
        void setTetrahedra(const mati &tetrahedra);
        void setFaces(const mati &faces);

        void translate(const vec3 &translation);
        void rotate(real angle, const vec3 &axis);

        /**
         * Begin tetrahedralization routine
         */
        void tetrahedralizeMesh();

        // Internal computation for functionality
        void computeDeformationGradients();

        auto computeTetrahedralVolumes() -> std::vector<real>;
        auto computeForces() -> vec;
        auto computeHessian() -> spmat;
        auto computeMassMatrix() -> spmat;
        auto computeStiffnessMatrix() -> spmat;
        auto computeVolumetricMassMatrix() -> spmat;
        auto computeDensityVolumeMassMatrix() -> spmat;
        auto computeDampingMatrix(const spmat &massMatrix, const spmat &stiffnessMatrix) -> spmat;

    private:
        real rayleighAlpha_ = 0.01;
        real rayleighBeta_ = 0.01;

        mat vertices_;
        mat restVertices_;
        mati tetrahedra_;
        mati faces_;

        std::vector<vec3i> surfaceTriangles_;
        std::vector<u32> surfaceVertices_;
        std::vector<u32> surfaceTetrahedra_;
        std::vector<vec2i> surfaceEdges_;

        // All deformation gradients for each tet
        std::vector<mat3> deformationGradients_;

        // All velocity gradients for each tet
        std::vector<mat3> velocityGradients_;

        std::map<int, int> volumeToSurfaceID_;

        void computeSurfaceTriangles();
        void computeSurfaceVertices();
        void computeSurfaceEdges();
    };
}// namespace nm::geometry
