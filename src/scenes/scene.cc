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

    void Scene::addShape(const geometry::TetMesh *tetMesh) {
        // Add a new space for the mesh
        viewer.append_mesh();

        // Initialize data storage objects
        auto data = igl::opengl::ViewerData();

        // Set the mesh values
        data.set_mesh(tetMesh->vertices(), tetMesh->faces());

        // Push the mesh values into the data list for the next render pass
        viewer.data_list.push_back(data);
    }

    auto Scene::addShapeFromFile(const std::string &filename) -> geometry::TetMesh * {
        const auto filepath = kAssetsPath / filename;
        geometry::TetMesh *tetMesh = new geometry::TetMesh(filepath.string());
        addShape(tetMesh);
        return tetMesh;
    }
}// namespace nm::testing::scenes
