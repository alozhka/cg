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
        src/engine/EngineAssembly.hpp
        src/app/EngineApplication.h
        src/engine/CrankShaft.hpp)

target_link_libraries(engine glfw OpenGL::GL)
