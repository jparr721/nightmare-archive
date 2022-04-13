cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_GLFW /cmake)

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
        # nm
        src/geometry.cc
        src/renderer_utils.cc
        src/mesh.cc
        src/nm_math.h
        src/visualization.cc
        src/visualization.h
        src/simulation.cc
        src/simulation.h
        src/mouse_control.cc
        src/mouse_control.h

        # nm::fem
        src/fem/phi_linear_tetrahedron.cc
        src/fem/phi_linear_tetrahedron.h
        src/fem/dphi_linear_tetrahedron_dX.cc
        src/fem/dphi_linear_tetrahedron_dX.h
        src/fem/psi_neo_hookean.cc
        src/fem/psi_neo_hookean.h
        src/fem/dpsi_neo_hookean_dF.cc
        src/fem/dpsi_neo_hookean_dF.h
        src/fem/d2psi_neo_hookean_dF2.cc
        src/fem/d2psi_neo_hookean_dF2.h
        src/fem/T_linear_tetrahedron.cc
        src/fem/T_linear_tetrahedron.h
        src/fem/V_linear_tetrahedron.cc
        src/fem/V_linear_tetrahedron.h
        src/fem/dV_linear_tetrahedron_dq.cc
        src/fem/dV_linear_tetrahedron_dq.h
        src/fem/d2V_linear_tetrahedron_dq2.cc
        src/fem/d2V_linear_tetrahedron_dq2.h
        src/fem/integrators_implicit_euler.h
        src/fem/assemble.cc
        src/fem/assemble.h
        src/fem/mass_matrix_linear_tetrahedron.cc
        src/fem/mass_matrix_linear_tetrahedron.h
        src/fem/mass_matrix_linear_tetmesh.cc
        src/fem/mass_matrix_linear_tetmesh.h
        src/fem/newtons_method.h
        src/fem/linear_energy_function.cc
        src/fem/linear_energy_function.h
        src/fem/spring_potential.cc
        src/fem/spring_potential.h src/fem/quadrature_single_point.h)

target_link_libraries(${PROJECT_LIB}
        PUBLIC
        igl::glfw
        igl::imgui
        igl::opengl
        igl_copyleft::tetgen
        spdlog
        )
add_executable(${PROJECT_NAME} src/nightmare.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})