#include "tet_mesh.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <spdlog/spdlog.h>

namespace nm::geometry {
    struct TriangleCompare {
        bool operator()(const vec3 &lhs, const vec3 &rhs) {
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
        igl::read_triangle_mesh(meshPath, vertices_, faces_);
        tetrahedralizeMesh();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &faces) : vertices_(vertices), faces_(faces) {
        tetrahedralizeMesh();
    }

    TetMesh::TetMesh(const mat &vertices, const mati &tetrahedra, const mati &faces)
        : vertices_(vertices), tetrahedra_(tetrahedra), faces_(faces) {}

    void TetMesh::setVertices(const mat &vertices) { vertices_ = vertices; }
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

    /**
     * Implementation stolen from Ted Kim's HOBAK. Thans Ted!
     */
    void TetMesh::computeSurfaceTriangles() {
        std::map<vec3i, int, TriangleCompare> faceCounts;
        for (auto ii = 0; ii < tetrahedra_.rows(); ++ii) {

            const vec4i t = tetrahedra_.row(ii);
            vec3i faces[4];
            faces[0] << t[0], t[1], t[3];
            faces[1] << t[0], t[2], t[1];
            faces[2] << t[0], t[3], t[2];
            faces[3] << t[1], t[2], t[3];

            for (int jj = 0; jj < 4; ++jj) { std::sort(faces[jj].data(), faces[jj].data() + faces[jj].size()); }

            for (int jj = 0; jj < 4; ++jj) { faceCounts[faces[jj]]++; }
        }

        // go back through the tets, if any of its faces have a count less than
        // 2, then it must be because it faces outside
        surfaceTriangles_.clear();
        for (auto ii = 0; ii < tetrahedra_.size(); ii++) {
            const vec4i t = tetrahedra_.row(ii);

            vec3i faces[4];

            // these are consistently ordered counter-clockwise
            faces[0] << t[0], t[1], t[3];
            faces[1] << t[0], t[2], t[1];
            faces[2] << t[0], t[3], t[2];
            faces[3] << t[1], t[2], t[3];

            vec3i facesSorted[4];

            // make a sorted copy, but keep the original around for rendering
            for (int jj = 0; jj < 4; ++jj) {
                facesSorted[jj] = faces[jj];
                std::sort(facesSorted[jj].data(), facesSorted[jj].data() + facesSorted[jj].size());
            }

            // see which faces don't have a dual
            for (int jj = 0; jj < 4; ++jj) {
                if (faceCounts[facesSorted[jj]] < 2) { surfaceTriangles_.push_back(faces[jj]); }
            }
        }

        spdlog::debug("Found {} surface triangles out of {} possible.", surfaceTriangles_.size(), tetrahedra_.rows());
    }

    /**
     * Implementation stolen from Ted Kim's HOBAK. Thans Ted!
     */
    void TetMesh::computeSurfaceVertices() {
        assert(!surfaceTriangles_.empty() && "NO SURFACE TRIANGLES FOUND!");

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
    void TetMesh::computeSurfaceEdges() {}

}// namespace nm::geometry
