cmake_minimum_required(VERSION 4.2)
project(ripple)

set(CMAKE_CXX_STANDARD 20)

add_executable(ripple main.cpp src/stb_image_impl.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(ripple PRIVATE graphics)

add_custom_target(
        ripple_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:ripple>/assets
)
add_dependencies(ripple ripple_copy_assets)