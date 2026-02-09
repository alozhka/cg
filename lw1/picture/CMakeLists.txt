cmake_minimum_required(VERSION 4.0)
project(picture)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS Graphics REQUIRED)

add_executable(picture main.cpp EricCartman.hpp DrawableObject.hpp)

target_link_libraries(picture SFML::Graphics)