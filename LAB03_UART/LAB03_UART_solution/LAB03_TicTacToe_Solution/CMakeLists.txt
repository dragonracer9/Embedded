cmake_minimum_required(VERSION 3.20)

# Generate compile_commands.json:
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("LAB03_TicTacToe_Solution" C CXX ASM)

include(cmake/st-project.cmake)

add_executable(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME} PRIVATE
    Core/Src/tictactoe.c
    Core/Src/the_computer.c
)

# Enable additional warnings:
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Copy compile_commands.json to project root:
add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_LIST_DIR}/
)
