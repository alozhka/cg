cmake_minimum_required(VERSION 4.0)
project(klein_bottle)

set(CMAKE_CXX_STANDARD 20)

add_executable(klein_bottle main.cpp
        src/KleinBottleApp.hpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(klein_bottle PRIVATE graphics)
graphics_copy_shader_assets(klein_bottle)