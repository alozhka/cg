cmake_minimum_required(VERSION 3.20)
project(hangman)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS Graphics REQUIRED)

include_directories(src/view)
include_directories(src)

add_executable(hangman
        main.cpp
        src/view/GameView.cpp
        src/view/AlphabetView.cpp
        src/view/Drawer.cpp
)

target_link_libraries(hangman SFML::Graphics)
