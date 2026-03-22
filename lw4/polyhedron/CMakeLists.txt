cmake_minimum_required(VERSION 4.0)
project(polyhedron)

set(CMAKE_CXX_STANDARD 20)

add_executable(polyhedron main.cpp
        src/PolyhedronApp.h)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)
target_link_libraries(polyhedron PRIVATE graphics)