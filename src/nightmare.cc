#include "camera.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


int main(int argc, char **argv) {
    using namespace nm;
    const auto window = std::make_unique<Window>("Nightmare");

    window->initialize();
    return window->launch();
}

// static void glfw_error_callback(int error, const char *description) {
//     fprintf(stderr, "Glfw Error %d: %s\n", error, description);
// }

// int main(int argc, char **argv) {
//     // Setup window
//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit()) return 1;

//         // Decide GL+GLSL versions
// #if defined(IMGUI_IMPL_OPENGL_ES2)
//     // GL ES 2.0 + GLSL 100
//     const char *glsl_version = "#version 100";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
// #elif defined(__APPLE__)
//     // GL 3.2 + GLSL 150
//     const char *glsl_version = "#version 150";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #else
//     // GL 3.0 + GLSL 130
//     const char *glsl_version = "#version 130";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     // Create window with graphics context
//     GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
//     if (window == NULL) { return EXIT_FAILURE; }

//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);// Enable vsync

//     // glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO &io = ImGui::GetIO();
//     (void) io;

//     ImGui::StyleColorsDark();

//     // Setup Platform/Renderer backends
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     // Our state
//     bool show_demo_window = true;
//     bool show_another_window = false;
//     ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//     // Main loop
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();

//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

//         {
//             static float f = 0.0f;
//             static int counter = 0;

//             ImGui::Begin("Hello, world!");

//             ImGui::Text("This is some useful text.");
//             ImGui::Checkbox("Demo Window", &show_demo_window);
//             ImGui::Checkbox("Another Window", &show_another_window);

//             ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
//             ImGui::ColorEdit3("clear color", (float *) &clear_color);

//             if (ImGui::Button("Button")) { counter++; }
//             ImGui::SameLine();
//             ImGui::Text("counter = %d", counter);

//             ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
//                         ImGui::GetIO().Framerate);
//             ImGui::End();
//         }

//         if (show_another_window) {
//             ImGui::Begin("Another Window", &show_another_window);
//             ImGui::Text("Hello from another window!");
//             if (ImGui::Button("Close Me")) show_another_window = false;
//             ImGui::End();
//         }

//         // Rendering
//         ImGui::Render();
//         int display_w, display_h;
//         glfwGetFramebufferSize(window, &display_w, &display_h);
//         glViewport(0, 0, display_w, display_h);
//         glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
//                      clear_color.w);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//         glfwSwapBuffers(window);
//     }

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();

//     glfwDestroyWindow(window);
//     glfwTerminate();

//     return EXIT_SUCCESS;
// }
