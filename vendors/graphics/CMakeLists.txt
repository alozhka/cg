cmake_minimum_required(VERSION 4.0)
project(graphics)

set(CMAKE_CXX_STANDARD 20)

# glfw
include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW ON
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

# OpenGL
find_package(OpenGL REQUIRED)

add_library(graphics INTERFACE)
target_include_directories(graphics INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(graphics INTERFACE glad glfw OpenGL::GL)
