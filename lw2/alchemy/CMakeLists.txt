cmake_minimum_required(VERSION 4.0)
project(alchemy)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS Graphics REQUIRED)

include_directories(src)

add_executable(alchemy main.cpp
        src/shared/IObserver.h
        src/shared/Observable.h
        src/model/AlchemyModel.h
        src/model/Element.h
        src/model/ConcreteElements.h
        src/model/RecipeBook.h)

target_link_libraries(alchemy SFML::Graphics)
