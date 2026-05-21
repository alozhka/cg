cmake_minimum_required(VERSION 4.0)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 20)

add_executable(ray_tracing main.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(ray_tracing PRIVATE graphics)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(ray_tracing PRIVATE OpenMP::OpenMP_CXX)
endif()

add_custom_target(
        ray_tracing_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:ray_tracing>/assets
)
add_dependencies(ray_tracing ray_tracing_copy_assets)