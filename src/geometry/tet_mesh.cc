#include "tet_mesh.h"
#include "../hyperelastic/stable_neo_hookean.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/parallel_for.h>
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
        initialize();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &faces)
        : vertices_(vertices), restVertices_(vertices_), faces_(faces) {
        tetrahedralizeMesh();
        initialize();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &tetrahedra, const mati &faces)
        : vertices_(vertices), tetrahedra_(tetrahedra), faces_(faces) {
        assert(vertices.rows() > 0 && "NO VERTICES PROVIDED");
        initialize();
    }

    void TetMesh::initialize() {
        // Compute tetrahedral volumes for the rest vertices.
        assert(restVertices_.rows() > 0 && "NO REST VERTICES FOUND");
        restTetVolumes_ = computeTetrahedralVolumes();
        restOneRingTetVolumes_ = computeTetrahedralRingVolumes();

        if (restTetVolumes_.empty() || restOneRingTetVolumes_.empty()) {
            spdlog::error("Failed to compute rest tetrahedral volumes");
            isInit = false;
            return;
        }

        // Compute dm inverses. Fail if not present
        computeDmInverses();
        if (dmInverses_.empty()) {
            spdlog::error("Failed to compute inversion of Dm");
            isInit = false;
            return;
        }

        // Compute the deformation gradients
        computeDeformationGradients();
        if (deformationGradients_.empty()) {
            spdlog::error("Failed to compute deformation gradients!");
            isInit = false;
            return;
        }

        // Compute the pF px matrices
        computePartialFPartialxs();
        if (pFpxs_.empty()) {
            spdlog::error("Failed to compute pFpx matrices!");
            isInit = false;
            return;
        }

        // Crash if any conditions fail (only on debug mode).
        assert(dmInverses_.size() > 0 && "INVERSES FAILED TO COMPUTE");

        // If we make it to the end, we have initialized successfully.
        isInit = true;
    }

    auto TetMesh::displacement() const -> vec {
        vec delta(ndofs());
        delta.setZero();

        for (int ii = 0; ii < vertices_.rows(); ++ii) {
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
        for (int ii = 0; ii < vertices_.rows(); ++ii) { vertices_.row(ii) = positions.segment(ii * 3, 3); }
    }

    void TetMesh::setDisplacement(const vec &delta) {
        assert(delta.rows() == ndofs() && "DELTA TOO SMALL");
        for (int ii = 0; ii < vertices_.rows(); ++ii) {
            vertices_.row(ii) = restVertices_.row(ii) + delta.segment(ii * 3, 3);
        }
    }

    void TetMesh::addDisplacement(const vec &delta) {
        assert(delta.rows() == ndofs() && "DELTA TOO SMALL");
        for (int ii = 0; ii < vertices_.rows(); ++ii) { vertices_.row(ii) += delta.segment(ii * 3, 3); }
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

    void TetMesh::computeDeformationGradients() {
        assert(!dmInverses_.empty() && "NO INVERSES FOUND");
        deformationGradients_ = std::vector<mat3>(tetrahedra_.rows());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this](const int index) {
                    deformationGradients_.at(index) =
                            utils::computeF(vertices_, dmInverses_.at(index), tetrahedra_.row(index));
                },
                kForceMaxThreadSaturation);
    }

    void TetMesh::computePartialFPartialxs() {
        assert(!dmInverses_.empty() && "NO INVERSES FOUND");
        pFpxs_ = std::vector<mat9x12>(tetrahedra_.rows());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this](const int index) { pFpxs_.at(index) = identities::partialFpartialx(dmInverses_.at(index)); },
                kForceMaxThreadSaturation);
    }

    auto TetMesh::computeTetrahedralVolumes() -> std::vector<real> {
        std::vector<real> volumes(tetrahedra_.rows());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &volumes](const int index) {
                    const vec4i row = tetrahedra_.row(index);
                    volumes.at(index) = utils::tetVolume({
                            vertices_.row(row(0)),
                            vertices_.row(row(1)),
                            vertices_.row(row(2)),
                            vertices_.row(row(3)),
                    });
                },
                kForceMaxThreadSaturation);

        return volumes;
    }

    auto TetMesh::computeTetrahedralRingVolumes() -> std::vector<real> {
        const auto tetVolumes = computeTetrahedralVolumes();

        // Start with zeros since we are accumulating
        std::vector<real> oneRingVolumes(vertices_.rows(), 0.0);

        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &tetVolumes, &oneRingVolumes](const int index) {
                    const real quarter = 0.25 * tetVolumes.at(index);
                    // Increment each volume for the tet by a quarter of its volume
                    // Unrolled for perf.
                    oneRingVolumes.at(tetrahedra_(index, 0)) += quarter;
                    oneRingVolumes.at(tetrahedra_(index, 1)) += quarter;
                    oneRingVolumes.at(tetrahedra_(index, 2)) += quarter;
                    oneRingVolumes.at(tetrahedra_(index, 3)) += quarter;
                },
                kForceMaxThreadSaturation);

        return oneRingVolumes;
    }

    auto TetMesh::computeForces(real lambda, real mu) -> vec {
        std::vector<vec12> perElementForces(tetrahedra_.rows());

        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &perElementForces, &lambda, &mu](const int index) {
                    const mat3 &F = deformationGradients_.at(index);
                    const mat3 pk1 = hyperelastic::pk1(F, lambda, mu);
                    const vec12 forceDensity = pFpxs_.at(index).transpose() * utils::vectorize(pk1);
                    const vec12 force = -restTetVolumes_.at(index) * forceDensity;
                    perElementForces.at(index) = force;
                },
                kForceMaxThreadSaturation);

        // Scatter the forces into a sparse vector
        vec forces = vec::Zero(ndofs());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &forces, &perElementForces](const int index) {
                    const vec4i tet = tetrahedra_.row(index);
                    const vec12 tetForce = perElementForces.at(index);
                    for (int jj = 0; jj < 4; ++jj) {
                        int index = 3 * tet(jj);
                        forces(index) += tetForce(3 * jj);
                        forces(index + 1) += tetForce(3 * jj + 1);
                        forces(index + 2) += tetForce(3 * jj + 2);
                    }
                },
                kForceMaxThreadSaturation);

        return forces;
    }

    auto TetMesh::computeHessian(real lambda, real mu) -> spmat {
        std::vector<mat12> perElementHessians(tetrahedra_.rows());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &perElementHessians, &lambda, &mu](const int index) {
                    const mat3 &F = deformationGradients_.at(index);
                    mat3 U, V;
                    vec3 sigma;
                    utils::computeRotationInvariantSVD(F, U, sigma, V);
                    const mat9x12 &pFpx = pFpxs_.at(index);
                    const mat9 hessian = -restTetVolumes_.at(index) * hyperelastic::dpk1(U, sigma, V, lambda, mu);
                    perElementHessians.at(index) = (pFpx.transpose() * hessian) * pFpx;
                },
                kForceMaxThreadSaturation);

        // Build out the sparse matrix from triplets
        std::vector<triplet> triplets;
        igl::parallel_for(
                tetrahedra_.rows(),
                [this, &triplets, &perElementHessians](const int index) {
                    const vec4i &tet = tetrahedra_.row(index);
                    const mat12 &H = perElementHessians.at(index);
                    for (int ii = 0; ii < 4; ++ii) {
                        for (int jj = 0; jj < 4; ++jj) {
                            for (int a = 0; a < 3; ++a) {
                                for (int b = 0; b < 3; ++b) {
                                    const real entry = H(3 * ii + a, 3 * jj + b);
                                    triplet t(3 * tet(ii) + a, 3 * tet(jj) + b, entry);
                                    triplets.push_back(t);
                                }
                            }
                        }
                    }
                },
                kForceMaxThreadSaturation);

        spmat hessian(ndofs(), ndofs());
        hessian.setFromTriplets(triplets.begin(), triplets.end());
        return hessian;
    }

    auto TetMesh::computeStiffnessMatrix() -> spmat {
        std::vector<mat12> perElementHessians(tetrahedra_.rows());
        for (int ii = 0; ii < tetrahedra_.rows(); ++ii) {
            const mat3 &F = deformationGradients_.at(ii);
            const mat9x12 &pFpx = pFpxs_.at(ii);
        }
    }

    auto TetMesh::computeVolumetricMassMatrix() -> spmat {
        std::vector<triplet> triplets;
        const std::vector<real> volumes = computeTetrahedralRingVolumes();
        // Set the diagonal one-ring voulmes
        for (int ii = 0; ii < vertices_.rows(); ++ii) {
            const auto volume = volumes.at(ii);

            // Assign triplet for this matrix entry and set the uniform volume
            triplets.emplace_back(3 * ii, 3 * ii, volume);
            triplets.emplace_back(3 * ii + 1, 3 * ii + 1, volume);
            triplets.emplace_back(3 * ii + 2, 3 * ii + 2, volume);
        }
        spmat M(ndofs(), ndofs());
        M.setFromTriplets(triplets.begin(), triplets.end());
        return M;
    }

    auto TetMesh::computeDensityVolumeMassMatrix() -> spmat {
        spdlog::error("{} Not implemented! Need to debug it some more", __FUNCTION__);
        assert(false);
    }

    auto TetMesh::computeDampingMatrix(const spmat &massMatrix, const spmat &stiffnessMatrix) -> spmat {
        const vec lastDisplacement = displacement();

        // Set the displacement to zero to get the rest state damping
        setDisplacement(vec::Zero(ndofs()));

        // Compute the stiffness matrix
        const spmat K = computeStiffnessMatrix();

        // Restore the old displacement back to what it was
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
        for (int ii = 0; ii < tetrahedra_.rows(); ++ii) {
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
        for (int ii = 0; ii < tetrahedra_.rows(); ii++) {
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
        for (int ii = 0; ii < surfaceTriangles_.size(); ++ii) {
            for (int jj = 0; jj < 3; ++jj) { foundVertices.at(surfaceTriangles_.at(ii)(jj)) = true; }
        }

        surfaceVertices_.clear();
        for (const auto &[vertex, _wasFound] : foundVertices) { surfaceVertices_.push_back(vertex); }

        // Compute the reverse lookup table
        for (int ii = 0; ii < surfaceVertices_.size(); ++ii) {
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
        for (int ii = 0; ii < surfaceTriangles_.size(); ++ii) {
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

    void TetMesh::computeDmInverses() {
        // Clear the vector
        dmInverses_ = std::vector<mat3>(tetrahedra_.rows());
        igl::parallel_for(
                tetrahedra_.rows(),
                [this](const int index) {
                    const vec4i tet = tetrahedra_.row(index);
                    const vec3 x0 = vertices_.row(tet(0));
                    const vec3 x1 = vertices_.row(tet(1));
                    const vec3 x2 = vertices_.row(tet(2));
                    const vec3 x3 = vertices_.row(tet(3));
                    dmInverses_.at(index) = utils::computeDMatrix(x0, x1, x2, x3).inverse();
                },
                kForceMaxThreadSaturation);
    }

}// namespace nm::geometry
