cmake_minimum_required(VERSION 4.0)
project(sea_battle)

set(CMAKE_CXX_STANDARD 20)

add_executable(sea_battle main.cpp
        src/SeaBattleApp.hpp
        src/std_image_impl.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(sea_battle PRIVATE graphics)

graphics_copy_shader_assets(sea_battle)

add_custom_command(
        TARGET sea_battle PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:sea_battle>/assets
)
