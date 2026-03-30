cmake_minimum_required(VERSION 4.0)
project(mobius_strip)

set(CMAKE_CXX_STANDARD 20)

add_executable(mobius_strip main.cpp
        src/MobiusStripApp.hpp
        src/MobiusStrip.hpp)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)
target_link_libraries(mobius_strip PRIVATE graphics)

add_custom_command(
        TARGET mobius_strip PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:mobius_strip>/assets
)