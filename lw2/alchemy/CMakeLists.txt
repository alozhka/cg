cmake_minimum_required(VERSION 4.0)
project(alchemy)

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
        src/viewModel/AlchemyViewModel.h
        src/shared/Uuid.h
        src/view/ElementCardView.h
        src/view/DeleteButtonView.h
        src/view/WorkspaceArea.h
        src/view/TextureCache.h)

target_link_libraries(alchemy SFML::Graphics)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
