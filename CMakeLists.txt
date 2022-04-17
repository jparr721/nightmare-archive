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
        src/fem/linear_tetrahedron_basis_functions.cc
        src/fem/linear_tetrahedron_basis_functions.h
        src/fem/linear_tetrahedron_basis_function_gradient_matrix.cc
        src/fem/linear_tetrahedron_basis_function_gradient_matrix.h
        src/fem/strain_energy_density_neo_hookean.cc
        src/fem/strain_energy_density_neo_hookean.h
        src/fem/strain_energy_density_neo_hookean_gradient_wrt_F.cc
        src/fem/strain_energy_density_neo_hookean_gradient_wrt_F.h
        src/fem/strain_energy_density_neo_hookean_hessian_wrt_F.cc
        src/fem/strain_energy_density_neo_hookean_hessian_wrt_F.h
        src/fem/linear_tetrahedron_kinetic_energy.cc
        src/fem/linear_tetrahedron_kinetic_energy.h
        src/fem/linear_tetrahedron_potential_energy.cc
        src/fem/linear_tetrahedron_potential_energy.h
        src/fem/linear_tetrahedron_potential_energy_gradient_wrt_q.cc
        src/fem/linear_tetrahedron_potential_energy_gradient_wrt_q.h
        src/fem/linear_tetrahedron_potential_energy_hessian_wrt_q.cc
        src/fem/linear_tetrahedron_potential_energy_hessian_wrt_q.h
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
        src/fem/linear_spring_potential_energy.cc
        src/fem/linear_spring_potential_energy.h src/fem/quadrature_single_point.h)

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
