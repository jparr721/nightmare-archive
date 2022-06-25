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
        bool isInit = false;

        explicit TetMesh(const std::string &meshPath);
        TetMesh(const mat &vertices, const mati &faces);
        TetMesh(const mat &vertices, const mati &tetrahedra, const mati &faces);

        void initialize();
        void initializeKinematic();

        auto vertices() -> mat & { return vertices_; }
        auto vertices() const -> const mat & { return vertices_; }
        auto tetrahedra() -> mati & { return tetrahedra_; }
        auto tetrahedra() const -> const mati & { return tetrahedra_; }
        auto faces() -> mati & { return faces_; }
        auto faces() const -> const mati & { return faces_; }
        /**
         * Computes the number of degrees of freedom (n vertices * 3)
         */
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
        void computePartialFPartialxs();

        auto computeTetrahedralVolumes() -> std::vector<real>;
        auto computeTetrahedralRingVolumes() -> std::vector<real>;
        auto computeForces(real lambda, real mu) -> vec;
        /**
         * Computes the stiffness matrix from the per-element hessians
         */
        auto computeStiffnessMatrix(real lambda, real mu) -> spmat;
        /**
         * Computes the mass matrix from the per-element masses
         */
        auto computeMassMatrix() -> spmat;
        /**
         * Computes the mass matrix from the one-ring volumes (assumes constant density)
         */
        auto computeVolumetricMassMatrix() -> spmat;
        auto computeDensityVolumeMassMatrix() -> spmat;
        /**
         * Computes the rayleigh damping matrix
         */
        auto computeDampingMatrix(const spmat &massMatrix, const spmat &stiffnessMatrix, real lambda, real mu) -> spmat;

    private:
        real rayleighAlpha_ = 0.01;
        real rayleighBeta_ = 0.01;

        mat vertices_;
        mat restVertices_;
        mati tetrahedra_;
        mati faces_;

        std::vector<real> restTetVolumes_;
        std::vector<real> restOneRingTetVolumes_;

        std::vector<vec3i> surfaceTriangles_;
        std::vector<u32> surfaceVertices_;
        std::vector<u32> surfaceTetrahedra_;
        std::vector<vec2i> surfaceEdges_;
        std::vector<bool> invertedVertices_;

        // Static memory allocation for the inverses of the Dm matrix for computing deformation gradient.
        std::vector<mat3> dmInverses_;

        // All deformation gradients for each tet
        std::vector<mat3> deformationGradients_;

        // All of the change-of-basis calculations for F to x
        std::vector<mat9x12> pFpxs_;

        std::map<int, int> volumeToSurfaceID_;

        void computeSurfaceTriangles();
        void computeSurfaceVertices();
        void computeSurfaceEdges();
        void computeDmInverses();
        void computeInvertedVertices();
    };
}// namespace nm::geometry
