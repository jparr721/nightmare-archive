cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_COPYLEFT_TETGEN "Build target igl_copyleft::tetgen" ON)
include(libigl)

find_package(OpenGL REQUIRED)

set(GLFW_INSTALL OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(third_party/glfw)

add_subdirectory(third_party/imgui)
target_link_libraries(imgui glfw)

add_subdirectory(third_party/glad)

option(BUILD_TESTING OFF)
option(EIGEN_BUILD_DOC OFF)
add_subdirectory(third_party/eigen3)

add_subdirectory(third_party/fast_obj)

option(NIGHTMARE_TESTS "Build the tests" OFF)
if (NIGHTMARE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

add_library(${PROJECT_LIB}
  src/camera.cc
  src/geometry.cc
  src/renderer.cc
  src/renderer_utils.cc
  src/shader_program.cc
  src/shader.cc
  src/input.cc
  src/window.cc
)
target_link_libraries(${PROJECT_LIB}
  PUBLIC
  glfw
  imgui
  glad
  fast_obj
  Eigen3::Eigen
  igl::core
  )
add_executable(${PROJECT_NAME} src/nightmare.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BUILD_DIR})