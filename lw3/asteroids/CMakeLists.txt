cmake_minimum_required(VERSION 4.0)
project(asteroids)

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

add_executable(asteroids main.cpp
        src/app/AsteroidsApplication.hpp
        src/app/GuiApplication.cpp
        src/app/GuiApplication.h
        src/graphics/ShaderProgram.hpp
        src/graphics/Drawable.hpp
        src/graphics/CompositeDrawable.hpp
        src/graphics/Mesh.hpp
        src/graphics/Mat3.hpp
        src/graphics/Vec2f.hpp
        src/graphics/Color.hpp
        src/model/Ship.hpp
        src/model/Asteroid.hpp
        src/model/Bullet.hpp
        src/model/Debris.hpp
        src/model/CollisionDetector.hpp
        src/model/GameModel.hpp
        src/viewmodel/InputState.hpp
        src/viewmodel/GameViewModel.hpp
        src/view/ShipView.hpp
        src/view/AsteroidView.hpp
        src/view/DebrisView.hpp
        src/view/GameView.hpp)

target_include_directories(asteroids PRIVATE ${GLAD_DIR}/include)
target_link_libraries(asteroids glfw glad OpenGL::GL)

add_custom_command(
        TARGET asteroids PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/assets
        $<TARGET_FILE_DIR:asteroids>/assets
)