cmake_minimum_required(VERSION 3.0)
project(photonGui)

add_subdirectory(lib)

add_library(photonGui STATIC src/window.cpp)
target_link_libraries(photonGui glfw libglew_static)
target_include_directories(photonGui PUBLIC include)

add_executable(basic test/basic.cpp)
target_link_libraries(basic photonGui)