cmake_minimum_required(VERSION 4.0)
project(maze)

set(CMAKE_CXX_STANDARD 20)

add_executable(maze main.cpp
        src/MazeApp.hpp
)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(maze PRIVATE graphics)
graphics_copy_shader_assets(maze)
