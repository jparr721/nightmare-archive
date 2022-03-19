#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

inline auto nmLogGLErrors(const char *fn) -> void {
    GLenum err;
    for (;;) {
        err = glGetError();

        if (err == GL_NO_ERROR) { break; }

        std::cerr << "Error in fn: " << fn << ": " << err << std::endl;
    }
}

#define NM_LOG_GL_ERRORS() nmLogGLErrors(__FUNCTION__)