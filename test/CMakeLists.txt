add_executable(basic basic.cpp)
target_link_libraries(basic photonGui)

add_executable(domTest domTest.cpp)
target_link_libraries(domTest photonGui)

add_executable(pybindTest pybindTest.cpp)
target_link_libraries(pybindTest PRIVATE pybind11::embed)

add_executable(parserTest parserTest.cpp)
target_link_libraries(parserTest photonGui)