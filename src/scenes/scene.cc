#include "scene.h"
#include "../geometry/primitives/grid.h"
#include "paths.h"

namespace nm::testing::scenes {
    Scene::Scene() {
        menu.callback_draw_viewer_menu = [&]() { menu.draw_viewer_menu(); };
        plugin.widgets.push_back(&menu);
        viewer.plugins.push_back(&plugin);
        viewer.core().background_color = Eigen::Vector4f(0.15, 0.15, 0.15, 1.0);
    }

    void Scene::addGrid(real startingy, real spacingScale, int size) {
        mat points;
        mati edges;
        geometry::primitives::grid_loadGeometry(spacingScale, startingy, size, points, edges);
        viewer.data().set_edges(points, edges, Eigen::RowVector3d(1.0, 1.0, 1.0));
    }

    void Scene::addShape(const std::shared_ptr<geometry::TetMesh> tetMesh) {
        // Add a new space for the mesh
        viewer.append_mesh();

        // Initialize data storage objects
        auto data = igl::opengl::ViewerData();

        // Set the mesh values
        data.set_mesh(tetMesh->vertices(), tetMesh->faces());

        // Push the mesh values into the data list for the next render pass
        viewer.data_list.push_back(data);
    }

    auto Scene::addShapeFromFile(const std::string &filename) -> std::shared_ptr<geometry::TetMesh> {
        const auto filepath = kAssetsPath / filename;
        const auto tetMesh = std::make_shared<geometry::TetMesh>(filepath.string());
        addShape(tetMesh);
        return tetMesh;
    }

    void Scene::updateVertexPositions(const vec &V) {
        mat newVertices(V.rows() / 3, 3);
        for (int ii = 0; ii < V.rows() / 3; ++ii) { newVertices.row(ii) = V.segment<3>(3 * ii).transpose(); }
        viewer.data_list[0].V = newVertices;

        // Tell the viewer to update
        viewer.data_list[0].dirty |= igl::opengl::MeshGL::DIRTY_POSITION;
    }
}// namespace nm::testing::scenes
