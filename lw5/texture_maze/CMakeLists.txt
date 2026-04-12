cmake_minimum_required(VERSION 4.0)
project(texture_maze)

set(CMAKE_CXX_STANDARD 20)

add_executable(texture_maze main.cpp src/stb_image_impl.cpp
        src/TextureMazeMesh.hpp
)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(texture_maze PRIVATE graphics)
graphics_copy_shader_assets(texture_maze)

add_custom_command(
        TARGET texture_maze POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:texture_maze>/assets
)