cmake_minimum_required(VERSION 4.0)
project(polyhedron)

set(CMAKE_CXX_STANDARD 20)

add_executable(polyhedron main.cpp
        src/PolyhedronApp.h
        ../../vendors/graphics/Size.hpp
        src/Cube.hpp
        src/ObjLoader.hpp
        src/SnubDodecahedron.hpp
)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)
target_link_libraries(polyhedron PRIVATE graphics)
graphics_copy_shader_assets(polyhedron)

add_custom_command(
        TARGET polyhedron PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:polyhedron>/assets
)