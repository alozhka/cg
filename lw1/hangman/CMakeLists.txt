cmake_minimum_required(VERSION 3.20)
project(hangman)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(hangman main.cpp
        src/HangmanModel.h)

# Link GLFW and OpenGL
# The target name provided by glfw3Config.cmake is usually 'glfw'
target_link_libraries(hangman glfw OpenGL::GL)
