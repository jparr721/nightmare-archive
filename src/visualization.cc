#include "visualization.h"
#include "renderer_utils.h"

namespace nm {
    // Menus and other non-exported stuff
    namespace {
        auto callbackDrawViewerMenu() -> void {
            // Mesh
            if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
                float w = ImGui::GetContentRegionAvail().x;
                float p = ImGui::GetStyle().FramePadding.x;
                if (ImGui::Button("Load##Mesh", ImVec2((w - p) / 2.f, 0))) {
                    viewer().open_dialog_load_mesh();
                    mesh()->vertices = viewer().data().V;
                    mesh()->faces = viewer().data().F;
                }
                ImGui::SameLine(0, p);
                if (ImGui::Button("Save##Mesh", ImVec2((w - p) / 2.f, 0))) { viewer().open_dialog_save_mesh(); }
            }
            // Viewing options
            if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::Button("Center object", ImVec2(-1, 0))) {
                    viewer().core().align_camera_center(viewer().data().V, viewer().data().F);
                }
                if (ImGui::Button("Snap canonical view", ImVec2(-1, 0))) { viewer().snap_to_canonical_quaternion(); }

                // Zoom
                ImGui::PushItemWidth(80 * menu().menu_scaling());
                ImGui::DragFloat("Zoom", &(viewer().core().camera_zoom), 0.05f, 0.1f, 20.0f);

                // Select rotation type
                int rotation_type = static_cast<int>(viewer().core().rotation_type);
                static Eigen::Quaternionf trackball_angle = Eigen::Quaternionf::Identity();
                static bool orthographic = true;
                if (ImGui::Combo("Camera Type", &rotation_type, "Trackball\0Two Axes\0002D Mode\0\0")) {
                    using RT = igl::opengl::ViewerCore::RotationType;
                    auto new_type = static_cast<RT>(rotation_type);
                    if (new_type != viewer().core().rotation_type) {
                        if (new_type == RT::ROTATION_TYPE_NO_ROTATION) {
                            viewer().core().trackball_angle = Eigen::Quaternionf::Identity();
                            viewer().core().orthographic = true;
                        } else if (viewer().core().rotation_type == RT::ROTATION_TYPE_NO_ROTATION) {
                            viewer().core().trackball_angle = trackball_angle;
                            viewer().core().orthographic = orthographic;
                        }
                        viewer().core().set_rotation_type(new_type);
                    }
                }
            }
            // Helper for setting viewport specific mesh options
            auto make_checkbox = [&](const char *label, unsigned int &option) {
                return ImGui::Checkbox(
                        label, [&]() { return viewer().core().is_set(option); },
                        [&](bool value) { return viewer().core().set(option, value); });
            };
            // Draw options
            if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen)) {
                if (ImGui::Checkbox("Face-based", &(viewer().data().face_based))) {
                    viewer().data().dirty = igl::opengl::MeshGL::DIRTY_ALL;
                }
                make_checkbox("Show texture", viewer().data().show_texture);
                if (ImGui::Checkbox("Invert normals", &(viewer().data().invert_normals))) {
                    viewer().data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
                }
                make_checkbox("Show overlay", viewer().data().show_overlay);
                make_checkbox("Show overlay depth", viewer().data().show_overlay_depth);
                ImGui::ColorEdit4("Background", viewer().core().background_color.data(),
                                  ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::ColorEdit4("Line color", viewer().data().line_color.data(),
                                  ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
                ImGui::DragFloat("Shininess", &(viewer().data().shininess), 0.05f, 0.0f, 100.0f);
                ImGui::PopItemWidth();
            }
        }
    }// namespace

    igl::opengl::glfw::Viewer viewer_;
    igl::opengl::glfw::imgui::ImGuiMenu menu_;
    igl::opengl::glfw::imgui::ImGuiPlugin plugin_;
    std::unique_ptr<Mesh> mesh_;

    auto viewer() -> igl::opengl::glfw::Viewer & { return viewer_; }
    auto menu() -> igl::opengl::glfw::imgui::ImGuiMenu & { return menu_; }
    auto plugin() -> igl::opengl::glfw::imgui::ImGuiPlugin & { return plugin_; }
    auto mesh() -> std::unique_ptr<Mesh> & { return mesh_; }

    auto initialize() -> bool {
        matXr points;
        matXi edges;
        nm::makeRenderableGrid(1.0, 10, points, edges, -1.0);
        viewer().data().set_edges(points, edges, Rowvec3r(1.0, 1.0, 1.0));

        mesh_ = std::make_unique<Mesh>("assets/cube.obj");
        plugin().widgets.push_back(&menu());
        viewer().plugins.push_back(&plugin());
        menu().callback_draw_viewer_menu = &callbackDrawViewerMenu;
        viewer().data().set_mesh(mesh()->vertices, mesh()->faces);
        return true;
    }
}// namespace nm