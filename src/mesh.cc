#include "mesh.h"
#include <igl/copyleft/tetgen/tetrahedralize.h>
#include <igl/read_triangle_mesh.h>
#include <spdlog/spdlog.h>

namespace nm {
    void initializeMesh(const std::string &meshfile, Mesh &mesh) {
        igl::read_triangle_mesh(meshfile, mesh.vertices, mesh.faces);
    }

    auto tetrahedralizeMesh(Mesh &mesh, const char *flags) -> bool {
        matXr TV;
        matXi TF, TT;

        const TetgenResult res = static_cast<const TetgenResult>(
                igl::copyleft::tetgen::tetrahedralize(mesh.vertices, mesh.faces, flags, TV, TT, TF));

        if (res != 0) {
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
                    break;
            }
            return false;
        }

        mesh.vertices = TV;

        // Reverse the faces to fix the winding from boundary_facets. This fixes the normals.
        mesh.faces = TF.rowwise().reverse().eval();

        mesh.tetrahedra = TT;

        return true;
    }

    void translateMesh(Mesh &mesh, const vec3r &amount) {
        for (int ii = 0; ii < amount.rows(); ++ii) { (mesh.vertices.col(ii).array() += amount(ii)).matrix(); }
    }
}// namespace nm