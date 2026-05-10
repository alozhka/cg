cmake_minimum_required(VERSION 4.2)
project(chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(chess main.cpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

target_link_libraries(chess PRIVATE graphics)