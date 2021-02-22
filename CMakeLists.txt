cmake_minimum_required(VERSION 3.0)
project(photonGui)

add_subdirectory(lib)

#turn on all warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE photonSrc src/*.cpp)
add_library(photonGui STATIC ${photonSrc})
target_link_libraries(photonGui glfw libglew_static)
target_include_directories(photonGui PUBLIC include)
target_include_directories(photonGui PRIVATE src)
target_compile_definitions(photonGui PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

add_subdirectory(test)