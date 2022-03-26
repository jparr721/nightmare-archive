cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_GLFW "Build target igl::glfw" ON)
option(LIBIGL_IMGUI "Build target igl::imgui" ON)
option(LIBIGL_OPENGL "Build target igl::opengl" ON)
include(libigl)

option(NIGHTMARE_TESTS "Build the tests" ON)
if (NIGHTMARE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif ()

add_library(${PROJECT_LIB}
        #        src/geometry.cc
        src/renderer_utils.cc
        # src/mesh.cc
        src/nm_math.h)
#        src/nm_io.h
#        src/nm_io.cc)

target_link_libraries(${PROJECT_LIB}
        PUBLIC
        igl::glfw
        igl::imgui
        igl::opengl
        )
add_executable(${PROJECT_NAME} src/nightmare.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BUILD_DIR})