#include "tet_mesh.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <spdlog/spdlog.h>

namespace nm::geometry {
    struct TriangleCompare {
        bool operator()(const vec3i &lhs, const vec3i &rhs) const {
            if (lhs(0) < rhs(0)) return true;
            if (lhs(0) > rhs(0)) return false;
            if (lhs(1) < rhs(1)) return true;
            if (lhs(1) > rhs(1)) return false;
            if (lhs(2) < rhs(2)) return true;
            if (lhs(2) > rhs(2)) return false;
            return false;
        }
    };

    TetMesh::TetMesh(const std::string &meshPath) {
        igl::read_triangle_mesh(meshPath, restVertices_, faces_);
        vertices_ = restVertices_;
        tetrahedralizeMesh();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &faces)
        : vertices_(vertices), restVertices_(vertices_), faces_(faces) {
        tetrahedralizeMesh();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &tetrahedra, const mati &faces)
        : vertices_(vertices), tetrahedra_(tetrahedra), faces_(faces) {}

    auto TetMesh::displacement() const -> vec {
        vec delta(ndofs());
        delta.setZero();

        for (auto ii = 0; ii < vertices_.rows(); ++ii) {
            const vec3 diff = vertices_.row(ii) - restVertices_.row(ii);
            delta.segment(ii * 3, 3) = diff;
        }

        return delta;
    }

    void TetMesh::setRayleighAlpha(real rayleighAlpha) { rayleighAlpha_ = rayleighAlpha; }
    void TetMesh::setRayleighBeta(real rayleighBeta) { rayleighBeta_ = rayleighBeta; }

    void TetMesh::setVertices(const mat &vertices) {
        vertices_ = vertices;
        restVertices_ = vertices;
    }

    void TetMesh::setVerticesFromPositions(const vec &positions) {
        assert(positions.rows() == ndofs() && "POSITIONS TOO SMALL");
        for (auto ii = 0; ii < vertices_.rows(); ++ii) { vertices_.row(ii) = positions.segment(ii * 3, 3); }
    }

    void TetMesh::setDisplacement(const vec &delta) {
        assert(delta.rows() == ndofs() && "DELTA TOO SMALL");
        for (auto ii = 0; ii < vertices_.rows(); ++ii) {
            vertices_.row(ii) = restVertices_.row(ii) + delta.segment(ii * 3, 3);
        }
    }

    void TetMesh::addDisplacement(const vec &delta) {
        assert(delta.rows() == ndofs() && "DELTA TOO SMALL");
        for (auto ii = 0; ii < vertices_.rows(); ++ii) { vertices_.row(ii) += delta.segment(ii * 3, 3); }
    }

    void TetMesh::setTetrahedra(const mati &tetrahedra) { tetrahedra_ = tetrahedra; }
    void TetMesh::setFaces(const mati &faces) { faces_ = faces; }

    void TetMesh::translate(const vec3 &translation) {
        for (int ii = 0; ii < vertices_.rows(); ++ii) { vertices_.row(ii) += translation; }
    }

    void TetMesh::rotate(real angle, const vec3 &axis) {
        const Eigen::AngleAxis rotation = Eigen::AngleAxis(angle, axis);
        const mat3 rotationMatrix = rotation.toRotationMatrix();
        vertices_ *= rotationMatrix;
    }

    void TetMesh::tetrahedralizeMesh() {
        // Do we have vertices?
        assert(vertices_.size() > 0 && "NO VERTICES FOUND");
        // Do we have faces?
        assert(faces_.size() > 0 && "NO FACES FOUND");

        {
            // Temporaries to hold tetrahedralized data
            mat TV;
            mati TT, TF;
            const TetgenResult res = static_cast<TetgenResult>(
                    igl::copyleft::tetgen::tetrahedralize(vertices_, faces_, kDefaultTetgenFlags, TV, TT, TF));

#ifndef NDEBUG
            switch (res) {
                case kTetgenFailedToConvert:
                    spdlog::error("Internal igl error occurred");
                    break;
                case kTetgenCrashed:
                    spdlog::error("Tetgen crashed");
                    break;
                case kTetgenFailedToCreateTets:
                    spdlog::error("Tets could not be created for some reason");
                    break;
                case kTetgenFailedToConvertToMatrix:
                    spdlog::error("Matrix conversion failed for some reason");
                    break;
                default:
                    spdlog::info("Tetrahedralization completed successfully");
                    break;
            }
#endif
            // Copy the memory into the vertices so the temporary can go out of scope
            vertices_ = TV;

            // Reverse the faces for the render so that way the normals are correct
            faces_ = TF.rowwise().reverse().eval();

            // Copy the memory in the the tets so the temporary can go out of scope
            tetrahedra_ = TT;

            spdlog::debug("Done generating tetrahedra");
        }
    }

    auto TetMesh::computeTetrahedralVolumes() -> std::vector<real> {
        std::vector<real> volumes;
        volumes.reserve(tetrahedra_.rows());
        for (int ii = 0; ii < tetrahedra_.rows(); ++ii) {
            const vec4i row = tetrahedra_.row(ii);
            volumes.push_back(utils::tetVolume({
                    vertices_.row(row(0)),
                    vertices_.row(row(1)),
                    vertices_.row(row(2)),
                    vertices_.row(row(3)),
            }));
        }

        return volumes;
    }

    auto TetMesh::computeForces() -> vec {}

    auto TetMesh::computeHessian() -> spmat {}

    auto TetMesh::computeMassMatrix() -> spmat {}

    auto TetMesh::computeStiffnessMatrix() -> spmat {}

    auto TetMesh::computeVolumetricMassMatrix() -> spmat {}

    auto TetMesh::computeDensityVolumeMassMatrix() -> spmat {}

    auto TetMesh::computeDampingMatrix(const spmat &massMatrix, const spmat &stiffnessMatrix) -> spmat {
        const vec lastDisplacement = displacement();

        // New displacement starts at zero for this timestep
        setDisplacement(vec::Zero(ndofs()));

        // Compute the stiffness matrix
        const spmat K = computeStiffnessMatrix();

        // Resotre the old displacement
        setDisplacement(lastDisplacement);

        // Build the damping matrix
        spmat C = rayleighAlpha_ * massMatrix;
        C += rayleighBeta_ * stiffnessMatrix;

        return C;
    }

    /**
     * Implementation stolen from Ted Kim's HOBAK. Thans Ted!
     */
    void TetMesh::computeSurfaceTriangles() {
        std::map<vec3i, int, TriangleCompare> faceCounts;
        for (auto ii = 0; ii < tetrahedra_.rows(); ++ii) {
            const vec4i t = tetrahedra_.row(ii);
            vec3i faces[4] = {
                    {t(0), t(1), t(3)},
                    {t(0), t(2), t(1)},
                    {t(0), t(3), t(2)},
                    {t(1), t(2), t(3)},
            };

            for (auto &face : faces) { std::sort(face.data(), face.data() + face.size()); }
            for (auto &face : faces) { ++faceCounts.at(face); }
        }

        // Go back through the tets, if any of its faces have a count less than
        // 2, then it must be because it faces outside.
        surfaceTriangles_.clear();
        for (auto ii = 0; ii < tetrahedra_.size(); ii++) {
            const vec4i t = tetrahedra_.row(ii);

            // these are consistently ordered counter-clockwise
            const vec3i faces[4] = {
                    {t(0), t(1), t(3)},
                    {t(0), t(2), t(1)},
                    {t(0), t(3), t(2)},
                    {t(1), t(2), t(3)},
            };

            vec3i facesSorted[4];

            // make a sorted copy, but keep the original around for rendering
            for (int jj = 0; jj < 4; ++jj) {
                facesSorted[jj] = faces[jj];
                std::sort(facesSorted[jj].data(), facesSorted[jj].data() + facesSorted[jj].size());
            }

            // See which faces don't have a dual
            for (int jj = 0; jj < 4; ++jj) {
                if (faceCounts[facesSorted[jj]] < 2 /* min face count */) { surfaceTriangles_.push_back(faces[jj]); }
            }
        }

        spdlog::debug("Found {} surface triangles out of {} possible.", surfaceTriangles_.size(), tetrahedra_.rows());
    }

    /**
     * Implementation stolen from Ted Kim's HOBAK. Thans Ted!
     */
    void TetMesh::computeSurfaceVertices() {
        if (surfaceTriangles_.empty()) {
            spdlog::error("No surface triangles found, generating");
            computeSurfaceTriangles();
        }

        // Map out all found vertices.
        std::map<int, bool> foundVertices;
        for (auto ii = 0; ii < surfaceTriangles_.size(); ++ii) {
            for (int jj = 0; jj < 3; ++jj) { foundVertices.at(surfaceTriangles_.at(ii)(jj)) = true; }
        }

        surfaceVertices_.clear();
        for (const auto &[vertex, _wasFound] : foundVertices) { surfaceVertices_.push_back(vertex); }

        // Compute the reverse lookup table
        for (auto ii = 0; ii < surfaceVertices_.size(); ++ii) {
            volumeToSurfaceID_.insert({surfaceVertices_.at(ii), ii});
        }

        spdlog::debug("Found {} vertices on the surface", surfaceVertices_.size());
    }

    /**
     * Implementation stolen from Ted Kim's HOBAK. Thans Ted!
     */
    void TetMesh::computeSurfaceEdges() {
        if (surfaceTriangles_.empty()) {
            spdlog::error("No surface triangles found, generating");
            computeSurfaceTriangles();
        }

        // Hash the edges
        std::map<std::pair<int, int>, bool> edges;
        for (auto ii = 0; ii < surfaceTriangles_.size(); ++ii) {
            for (auto jj = 0; jj < 3; ++jj) {
                const auto v0 = surfaceTriangles_.at(ii)(jj);
                const auto v1 = surfaceTriangles_.at(ii)(jj + 1 % 3);

                // Store them in sorted order
                std::pair<int, int> edge;
                if (v0 > v1) {
                    edge.first = v1;
                    edge.second = v0;
                } else {
                    edge.first = v0;
                    edge.second = v1;
                }

                edges.insert({edge, true});
            }
        }

        // Store all the unique hashes
        surfaceEdges_.clear();
        for (const auto &[edge, _] : edges) { surfaceEdges_.emplace_back(edge.first, edge.second); }

        spdlog::debug("Found {} edges on the surface", surfaceEdges_.size());
    }

}// namespace nm::geometry
