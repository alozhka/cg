cmake_minimum_required(VERSION 4.0)
project(polyhedron)

set(CMAKE_CXX_STANDARD 20)

add_executable(polyhedron main.cpp
        src/PolyhedronApp.h
        ../../vendors/graphics/Mat4.hpp
        ../../vendors/graphics/Size.hpp
        ../../vendors/graphics/Vec3.hpp
        src/Rectangle3d.hpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)
target_link_libraries(polyhedron PRIVATE graphics)
graphics_copy_shader_assets(polyhedron)