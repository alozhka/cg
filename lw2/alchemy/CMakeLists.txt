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
        src/model/RecipeBook.h
        src/view/GameView.h
        src/view/DiscoveredPanel.h
        src/view/Colors.h
        src/AlchemyViewModel.h
        src/shared/Uuid.h
        src/view/ElementCardView.h)

target_link_libraries(alchemy SFML::Graphics)
