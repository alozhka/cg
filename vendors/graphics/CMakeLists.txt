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

#glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glm)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

# OpenGL
find_package(OpenGL REQUIRED)

add_library(graphics INTERFACE)
target_include_directories(graphics INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(graphics INTERFACE glad glfw OpenGL::GL glm)

function(graphics_copy_shader_assets target_name)
    add_custom_command(
            TARGET ${target_name} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/shaders/assets
            $<TARGET_FILE_DIR:${target_name}>/assets
    )
endfunction()