cmake_minimum_required(VERSION 4.0)
project(china_flag)

set(CMAKE_CXX_STANDARD 20)

add_executable(china_flag main.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(china_flag PRIVATE graphics)

add_custom_target(
        china_flag_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:china_flag>/assets
)
add_dependencies(china_flag china_flag_copy_assets)