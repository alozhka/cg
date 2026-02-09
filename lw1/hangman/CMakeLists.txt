cmake_minimum_required(VERSION 3.20)
project(hangman)

set(CMAKE_CXX_STANDARD 20)

# Add Homebrew prefix for Apple Silicon if needed
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(src/view)
include_directories(src)

add_executable(hangman 
    main.cpp
    src/view/GameView.cpp
    # Add other sources if they are cpp files
)

# Link GLFW and OpenGL
target_link_libraries(hangman glfw OpenGL::GL)
