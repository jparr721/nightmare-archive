#include "mesh.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <igl/unique_simplices.h>
#include <spdlog/spdlog.h>

#include <utility>

namespace nm {
    Mesh::Mesh(const std::string &meshFile) { igl::read_triangle_mesh(meshFile, vertices, faces); }

    void tetrahedralizeMesh(Mesh *mesh, const char *flags) {
        matXr TV;
        matXi F, TF, TT;
        igl::unique_simplices(mesh->faces, F);

        const TetgenResult res = static_cast<const TetgenResult>(
                igl::copyleft::tetgen::tetrahedralize(mesh->vertices, F, flags, TV, TT, TF));

        if (res != 0) {
            switch (res) {
                case kTetgenFailedToConvert:
                    spdlog::error("Internal igl error occurred");
                case kTetgenCrashed:
                    spdlog::error("Tetgen crashed");
                case kTetgenFailedToCreateTets:
                    spdlog::error("Tets could not be created for some reason");
                case kTetgenFailedToConvertToMatrix:
                    spdlog::error("Matrix conversion failed for some reason");
                default:
                    break;
            }
        }

        mesh->vertices = TV;

        // Reverse the faces to fix the winding from boundary_facets. This fixes the normals.
        mesh->faces = TF.rowwise().reverse().eval();

        mesh->tetrahedra = TT;
    }

    void translateMesh(Mesh *mesh, const vec3r &amount) {
        for (int ii = 0; ii < amount.rows(); ++ii) { (mesh->vertices.col(ii).array() += amount(ii)).matrix(); }
    }
}// namespace nm