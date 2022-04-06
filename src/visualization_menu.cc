#include "visualization_menu.h"
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>

namespace nm {
    auto drawVisualizerMenu(VisualizationVars &vars, SimulationVars &simulationVars) -> void {
        // Mesh
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            float w = ImGui::GetContentRegionAvail().x;
            float p = ImGui::GetStyle().FramePadding.x;
            if (ImGui::Button("Load##Mesh", ImVec2((w - p) / 2.f, 0))) {
                vars.viewer.open_dialog_load_mesh();
                vars.mesh->vertices = vars.viewer.data().V;
                vars.mesh->faces = vars.viewer.data().F;
                tetrahedralizeMesh(vars.mesh.get());
                vars.viewer.data().set_mesh(vars.mesh->vertices, vars.mesh->faces);
            }
            ImGui::SameLine(0, p);
            if (ImGui::Button("Save##Mesh", ImVec2((w - p) / 2.f, 0))) { vars.viewer.open_dialog_save_mesh(); }
        }
        // Viewing options
        if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Center object", ImVec2(-1, 0))) {
                vars.viewer.core().align_camera_center(vars.viewer.data().V, vars.viewer.data().F);
            }
            if (ImGui::Button("Snap canonical view", ImVec2(-1, 0))) { vars.viewer.snap_to_canonical_quaternion(); }

            // Zoom
            ImGui::PushItemWidth(80 * vars.menu.menu_scaling());
            ImGui::DragFloat("Zoom", &(vars.viewer.core().camera_zoom), 0.05f, 0.1f, 20.0f);
        }
        // Helper for setting viewport specific mesh options
        auto makeCheckbox = [&](const char *label, unsigned int &option) {
            return ImGui::Checkbox(
                    label, [&]() { return vars.viewer.core().is_set(option); },
                    [&](bool value) { return vars.viewer.core().set(option, value); });
        };
        // Draw options
        if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Face-based", &(vars.viewer.data().face_based))) {
                vars.viewer.data().dirty = igl::opengl::MeshGL::DIRTY_ALL;
            }
            if (ImGui::Checkbox("Invert normals", &(vars.viewer.data().invert_normals))) {
                vars.viewer.data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
            }
            makeCheckbox("Show overlay", vars.viewer.data().show_overlay);
            makeCheckbox("Show overlay depth", vars.viewer.data().show_overlay_depth);
        }

        // Sim options
        if (ImGui::CollapsingHeader("Sim Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Simulating", &simulationVars.isSimulating)) {}
        }
    }
}// namespace nm