cmake_minimum_required(VERSION 4.0)
project(memory_trainer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../vendors/graphics
        ${CMAKE_CURRENT_BINARY_DIR}/graphics
)

add_executable(${PROJECT_NAME} main.cpp
        src/app/MemoryTrainerApp.hpp
        src/memory_trainer/model/MemoryTrainer.hpp
        src/memory_trainer/model/Card.hpp
        src/memory_trainer/view_model/MemoryTrainerViewModel.hpp
        src/memory_trainer/view_model/CardsViewModel.hpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE graphics)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)