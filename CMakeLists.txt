cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_GLFW "Build target igl::glfw" ON)
option(LIBIGL_IMGUI "Build target igl::imgui" ON)
option(LIBIGL_OPENGL "Build target igl::opengl" ON)
option(LIBIGL_COPYLEFT_TETGEN "Build target igl_copyleft::tetgen" ON)
include(libigl)

add_subdirectory(third_party/spdlog)

option(NIGHTMARE_TESTS "Build the tests" ON)
if (NIGHTMARE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif ()

add_library(${PROJECT_LIB}
    src/scenes/scene.h
    src/scenes/scene.cc

    src/geometry/primitives/grid.h
    src/geometry/primitives/grid.cc
    src/geometry/primitives/cube.h
    src/geometry/primitives/cube.cc

    src/geometry/tet_mesh.h
    src/geometry/tet_mesh.cc

    src/hyperelastic/neo_hookean.h
    src/hyperelastic/neo_hookean.cc
    src/hyperelastic/stable_neo_hookean.h
    src/hyperelastic/stable_neo_hookean.cc

    src/integrators/integrator.h
    src/integrators/integrator.cc
    src/integrators/backward_euler.h
    src/integrators/backward_euler.cc

    src/math.h
    )

target_link_libraries(${PROJECT_LIB}
        PUBLIC
        igl::glfw
        igl::imgui
        igl::opengl
        igl_copyleft::tetgen
        spdlog
        )

macro(package_add_scene SCENE_NAME FILE_NAME)
    add_executable(${SCENE_NAME} "src/scenes/${FILE_NAME}")
    target_link_libraries(${SCENE_NAME} ${PROJECT_LIB})
endmacro()

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

package_add_scene("drop" drop.cc)
