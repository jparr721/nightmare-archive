#include "mesh.h"
#include "nm_io.h"
#include <igl/list_to_matrix.h>
#include <igl/per_face_normals.h>
#include <igl/readMESH.h>
#include <igl/readOBJ.h>

#include <utility>

namespace nm {
    Mesh::Mesh(const std::string &mesh_file) {
        const auto extension = getFileExtension(mesh_file);
        assert(!extension.empty() && "EXTENSION IS INVALID");
        assert(extension == ".obj" || extension == ".mesh" && "INVALID MESH FILE SUPPLIED");

        if (extension == ".obj") {
            if (!igl::readOBJ(mesh_file, vertices, faces)) { std::cerr << "Reading obj file failed!" << std::endl; }
            //            vertices = stlVectorToEigenVector(flatten(V));
        } else if (extension == ".mesh") {
            std::vector<std::vector<real>> T;
            //            igl::readMESH(mesh_file, V, T, F);
            //            vertices = stlVectorToEigenVector(flatten(V));
            //            faces = stlVectorToEigenVector(flatten(F));
            //            tetrahedra = stlVectorToEigenVector(flatten(T));
        }

        // Compute normals
        rowMatXr N;
        igl::per_face_normals(vertices, faces, normals);
        normals = matrixToVector(N);

        // Compute colors
        vec3r default_color(255.0 / 255.0, 235.0 / 255.0, 80.0 / 255.0);
        colors = default_color.replicate(vertices.rows(), 1);
    }

    // TODO(@jparr721) - Add normals here.
    //    Mesh::Mesh(vecXr vertices, vecXr colors) : vertices(std::move(vertices)), colors(std::move(colors)) {}
    //
    //    Mesh::Mesh(vecXr vertices, vecXr colors, vecXr normals)
    //        : vertices(std::move(vertices)), colors(std::move(colors)), normals(std::move(normals)) {}
}// namespace nm