cmake_minimum_required(VERSION 3.0)
project(photonGui)

add_subdirectory(lib)

file(GLOB photonSrc src/*)
add_library(photonGui STATIC ${photonSrc})
target_link_libraries(photonGui glfw libglew_static)
target_include_directories(photonGui PUBLIC include)

add_subdirectory(test)