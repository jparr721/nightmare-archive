#include "window.h"
#include "input.h"
#include "resources.h"

namespace nm {
    integer window_width = 600;
    integer window_height = 400;

    GLuint shader_program;
    std::vector<GLuint> shader_ids;

    vec4r background_color = vec4r(0.16, 0.16, 0.16, 0.0);

    GLFWwindow *window;

    std::unique_ptr<Input> input = std::make_unique<Input>();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();

//    std::shared_ptr<ShaderProgram> shader_program;
    std::shared_ptr<Renderer> renderer;

    static void glfwErrorCallback(int error, const char *description) {
        std::cerr << "Glfw Broke (" << error << "): " << description << std::endl;
    }

    static void glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        input->handleCursorPos(window, xpos, ypos, camera);
    }

    static void glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        input->handleMouseButtonPress(window, button, action, mods, camera);
    }

    static void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        input->handleScrollEvent(xoffset, yoffset, camera);
    }

    static void glfwResizeEvent(GLFWwindow *window, int width, int height) {
        window_width = width;
        window_height = height;
        glViewport(0, 0, window_width, window_height);
        camera->resize(window_width, window_height);
        renderer->resize(window_width, window_height);
    }


    void destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    auto initialize(const std::string &window_title) -> bool {
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) { return false; }

        constexpr auto kGlslVersion = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        if (monitor == nullptr) {
            std::cerr << "Failed to get primary monitor" << std::endl;
            return false;
        }

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        if (mode == nullptr) {
            std::cerr << "Failed to get monitor" << std::endl;
            std::cerr << "Initialization failed" << std::endl;
            return false;
        }

        window_width = static_cast<integer>(mode->width * 0.75);
        window_height = static_cast<integer>(mode->height * 0.75);
        camera->resize(window_width, window_height);

        window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);

        if (window == nullptr) {
            std::cerr << "Failed to create window" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(window);

        // Enable vsync
        glfwSwapInterval(1);

        // Load all OpenGL function pointers
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "failed to initize GLAD" << std::endl;
            return false;
        }

        // Setup Imgui Context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        // Setup imgui backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(kGlslVersion);

        glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
        glfwSetScrollCallback(window, glfwScrollCallback);
        glfwSetCursorPosCallback(window, glfwCursorPosCallback);
        glfwSetWindowSizeCallback(window, glfwResizeEvent);

        shader_ids.push_back(compileShader(GL_VERTEX_SHADER, "shaders/core.vert.glsl"));
        shader_ids.push_back(compileShader(GL_FRAGMENT_SHADER, "shaders/core.frag.glsl"));
        shader_program = makeShaderProgram(shader_ids);
        glLinkProgram(shader_program);

        renderer = std::make_unique<Renderer>(shader_program, camera, RenderMode::kMesh);
        return true;
    }

    auto launch() -> int {
        camera->resize(window_width, window_height);
        renderer->resize(window_width, window_height);

        bool is_vis = true;

        auto mesh = std::make_unique<Mesh>();
        mesh->vertices.resize(12);
        mesh->vertices << 0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f;
        mesh->faces.resize(6);
        mesh->faces << 0, 1, 3, 1, 2, 3;
        mesh->colors.resize(12);
        mesh->colors << 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Hey", &is_vis);
            ImGui::Text("Sup");
            ImGui::End();

            ImGui::Render();

            glClearColor(background_color(0), background_color(1), background_color(2), background_color(3));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer->render(mesh);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        destroy();

        return EXIT_SUCCESS;
    }

}// namespace nm