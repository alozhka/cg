cmake_minimum_required(VERSION 4.2)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess
        main.cpp
        src/stb_image_impl.cpp
)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(chess PRIVATE graphics)

graphics_copy_shader_assets(chess)

add_custom_command(
        TARGET chess PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:chess>/assets
)