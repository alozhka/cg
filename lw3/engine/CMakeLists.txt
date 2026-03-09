cmake_minimum_required(VERSION 4.0)
project(engine)

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
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

# OpenGL
find_package(OpenGL REQUIRED)

add_executable(engine main.cpp
        src/app/GuiApplication.cpp
        src/graphics/Color.hpp
        src/graphics/Drawable.hpp
        src/graphics/CompositeDrawable.hpp
        src/shapes/Rectangle.hpp
        src/shapes/Circle.hpp
        src/engine/Engine.hpp
        src/app/EngineApplication.h
        src/engine/CrankShaft.hpp
        src/engine/Piston.hpp
        src/engine/SparkPlug.hpp
        src/engine/Valve.hpp
        src/engine/Pipe.hpp
        src/graphics/ShaderProgram.hpp
        src/graphics/Mat3.hpp
        src/graphics/Mesh.hpp
        src/graphics/Vec2f.hpp)

target_include_directories(engine PRIVATE ${GLAD_DIR}/include)
target_link_libraries(engine glfw glad OpenGL::GL)

add_custom_command(
        TARGET engine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/assets
        $<TARGET_FILE_DIR:engine>/assets
)