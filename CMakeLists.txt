cmake_minimum_required(VERSION 3.12...3.18)
project(nightmare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(third_party/glad)

add_subdirectory(third_party/glfw)

set(EIGEN_BUILD_TESTING OFF)
set(EIGEN_BUILD_DOC OFF)
add_subdirectory(third_party/eigen3)

add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw Eigen3::Eigen)

