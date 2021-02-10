cmake_minimum_required(VERSION 3.0)
project(photonGui)

add_subdirectory(lib)

#turn on all warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB photonSrc src/*)
add_library(photonGui STATIC ${photonSrc})
target_link_libraries(photonGui glfw libglew_static)
target_include_directories(photonGui PUBLIC include)

add_subdirectory(test)