#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <sstream>

inline auto nmLogGLErrors(const std::string &fn, const std::string &file, const int line) -> void {
    GLenum err;
    // gluErrorString was deprecated
    const auto gluErrorString = [](GLenum errorCode) -> const char * {
        switch (errorCode) {
            default:
                return "unknown error code";
            case GL_NO_ERROR:
                return "no error";
            case GL_INVALID_ENUM:
                return "invalid enumerant";
            case GL_INVALID_VALUE:
                return "invalid value";
            case GL_INVALID_OPERATION:
                return "invalid operation";
#ifndef GL_VERSION_3_0
            case GL_STACK_OVERFLOW:
                return "stack overflow";
            case GL_STACK_UNDERFLOW:
                return "stack underflow";
            case GL_TABLE_TOO_LARGE:
                return "table too large";
#endif
            case GL_OUT_OF_MEMORY:
                return "out of memory";
#ifdef GL_EXT_framebuffer_object
            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
                return "invalid framebuffer operation";
#endif
        }
    };
    for (;;) {
        err = glGetError();

        if (err == GL_NO_ERROR) { break; }

        std::ostringstream oss;
        oss << "GL_ERROR "
            << "[" << file << ":" << line << "]" << fn << "(): " << gluErrorString(err);

        std::cerr << oss.str() << std::endl;
        assert(false);
    }
}

#define NM_LOG_GL_ERRORS() nmLogGLErrors(__FUNCTION__, __FILE__, __LINE__)