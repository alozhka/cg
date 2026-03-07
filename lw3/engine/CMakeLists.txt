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
        src/shared/Color.hpp
        src/shared/SceneObject.hpp
        src/shared/CompositeObject.hpp
        src/shapes/Rectangle.hpp
        src/shared/Point.hpp
        src/shapes/Circle.hpp
        src/engine/Engine.hpp
        src/app/EngineApplication.h
        src/engine/CrankShaft.hpp
        src/engine/Piston.hpp
        src/engine/SparkPlug.hpp
        src/engine/Valve.hpp
        src/engine/Pipe.hpp
        src/render/ShaderProgram.hpp
        src/render/Mat3.hpp
        src/render/Mesh.hpp
        src/render/Vec2f.h)

target_include_directories(engine PRIVATE ${GLAD_DIR}/include)
target_link_libraries(engine glfw glad OpenGL::GL)

add_custom_command(
        TARGET engine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/render/assets
        $<TARGET_FILE_DIR:engine>/assets
)