#include "tet_mesh.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <spdlog/spdlog.h>

namespace nm::geometry {
#ifdef NDEBUG
    constexpr const char *kDefaultTetgenFlags = "zpqQ";
#else
    constexpr const char *kDefaultTetgenFlags = "zpq";
#endif

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

}// namespace nm::geometry
