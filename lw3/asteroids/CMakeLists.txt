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
        src/app/AsteroidsApplication.hpp
        src/app/GuiApplication.cpp
        src/graphics/ShaderProgram.hpp
        src/asteroids/model/AsteroidsGame.hpp
        src/asteroids/model/Spaceship.hpp
        src/asteroids/view/AsteroidsGameView.hpp
        src/asteroids/view/SpaceshipView.hpp
        src/asteroids/viewModel/AsteroidsGameViewModel.hpp
        src/asteroids/viewModel/SpaceshipViewModel.hpp
        src/graphics/Drawable.hpp
        src/app/KeyboardReader.hpp
        src/asteroids/model/Bullet.hpp
        src/asteroids/model/Coordinates.hpp
        src/asteroids/model/Debris.hpp
        src/asteroids/view/BulletsView.hpp
        src/asteroids/view/DebrisView.hpp
        src/asteroids/viewModel/BulletsViewModel.hpp
        src/asteroids/viewModel/DebrisViewModel.hpp
        src/asteroids/view/AsteroidsView.hpp
        src/asteroids/model/Asteroid.hpp
        src/asteroids/viewModel/AsteroidsViewModel.h
        src/asteroids/model/Randomizer.hpp
        src/asteroids/model/Collision.hpp
)

target_include_directories(asteroids PRIVATE ${GLAD_DIR}/include)
target_link_libraries(asteroids glfw glad OpenGL::GL)

add_custom_command(
        TARGET asteroids PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/assets
        $<TARGET_FILE_DIR:asteroids>/assets
)