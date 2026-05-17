cmake_minimum_required(VERSION 4.0)
project(mebius_to_klein)

set(CMAKE_CXX_STANDARD 20)

add_executable(mebius_to_klein main.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(mebius_to_klein PRIVATE graphics)

add_custom_target(
        mebius_to_klein_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:mebius_to_klein>/assets
)
add_dependencies(mebius_to_klein mebius_to_klein_copy_assets)