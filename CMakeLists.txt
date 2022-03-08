cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_subdirectory(third_party/imgui)

add_subdirectory(third_party/glad)

set(GLFW_INSTALL OFF)
add_subdirectory(third_party/glfw)

set(EIGEN_BUILD_TESTING OFF)
set(EIGEN_BUILD_DOC OFF)
add_subdirectory(third_party/eigen3)

add_subdirectory(third_party/fast_obj)

file(GLOB SOURCES src/*.cc)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  OpenGL
  imgui
  glad
  fast_obj
  glfw
  Eigen3::Eigen
)

