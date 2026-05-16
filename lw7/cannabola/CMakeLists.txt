cmake_minimum_required(VERSION 4.0)
project(cannabola)

set(CMAKE_CXX_STANDARD 20)

add_executable(cannabola main.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(cannabola PRIVATE graphics)

add_custom_target(
        cannabola_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:cannabola>/assets
)
add_dependencies(cannabola cannabola_copy_assets)