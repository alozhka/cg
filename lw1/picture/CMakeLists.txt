cmake_minimum_required(VERSION 4.0)
project(picture)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)
set(SFML_BUILD_TESTS OFF)

FetchContent_MakeAvailable(SFML)

add_executable(picture main.cpp EricCartman.hpp DrawableObject.hpp)

target_link_libraries(picture SFML::Graphics)