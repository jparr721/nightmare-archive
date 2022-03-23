#include "window.h"
#include "input.h"

namespace nm {
    const std::string kVertexShader =
            R"(#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 light;
uniform mat4 normal_matrix;

out vec3 surface_normal;
out vec4 ambient_color;
out vec3 light_dir;

void main() {
  vec4 vertex_pos = vec4(position, 1.0);

  light_dir = normalize(light - position);
  surface_normal = normalize(normal_matrix * vec4(normal, 1.0f)).xyz;

  gl_Position = projection * view * vertex_pos;

  ambient_color = vec4(color, 1.0f);
}
)";

    const std::string kFragmentShader =
            R"(#version 330

in vec3 surface_normal;
in vec4 ambient_color;
in vec3 light_dir;

out vec4 frag_color;

void main() {
  frag_color = ambient_color;

  vec4 specular_color = vec4(1.0f, 1.0f, 1.0f, 0.5f);
  vec4 diffuse_color = vec4(ambient_color.rgb, 0.5f);

  // Make this a var later.
  float shininess = 20.0f;

  float diffuse = max(0.0f, dot(normalize(surface_normal), normalize(light_dir)));
  vec4 diffuse_light = diffuse * diffuse_color;
  frag_color += diffuse_light;

  if (diffuse != 0.0f) {
    vec3 reflection = normalize(reflect(-normalize(light_dir), normalize(surface_normal)));

    float reflection_angle = max(0.0f, dot(normalize(surface_normal), reflection));

    float specular_exp = pow(reflection_angle, shininess);
    vec4 specular_light = specular_color * specular_exp;
    frag_color += specular_light;
  }
}
)";

    integer window_width = 600;
    integer window_height = 400;

    vec4r background_color = vec4r(0.16, 0.16, 0.16, 0.0);

    GLFWwindow *window;

    std::unique_ptr<Input> input = std::make_unique<Input>();

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();

    std::shared_ptr<ShaderProgram> shader_program;
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
        camera->resize(width, height);
        window_width = width;
        window_height = height;
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
        shader_program = std::make_shared<ShaderProgram>();

        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) { return false; }

#if defined(__APPLE__)
        constexpr auto kGlslVersion = "#version 150";
#else
        constexpr auto kGlslVersion = "#version 130";
#endif
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

        shader_program->addShader(GL_VERTEX_SHADER, kVertexShader.c_str());
        shader_program->addShader(GL_FRAGMENT_SHADER, kFragmentShader.c_str());

        glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
        glfwSetScrollCallback(window, glfwScrollCallback);
        glfwSetCursorPosCallback(window, glfwCursorPosCallback);
        glfwSetWindowSizeCallback(window, glfwResizeEvent);

        renderer = std::make_unique<Renderer>(shader_program, camera, RenderMode::kMeshAndLines);

        return true;
    }

    auto launch() -> int {
        camera->resize(window_width, window_height);
        renderer->resize(window_width, window_height);

        bool is_vis = true;

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

            renderer->render(nullptr);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        destroy();

        return EXIT_SUCCESS;
    }
}// namespace nm