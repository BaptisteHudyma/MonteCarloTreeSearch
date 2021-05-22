cmake_minimum_required(VERSION 3.1)

project(MCTS)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-O2 -Wall")
set(CMAKE_BUILD_TYPE Debug)
MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})


set(SRC src)
set(GAMES games)

include_directories(
    ${SRC}
    ${GAMES}
)

add_executable(mcts
    main.cpp
)

add_library(Games
    ${GAMES}/tictactoe.cpp
    ${GAMES}/puissance4.cpp
)

add_library(TreeSearch
    ${SRC}/node.cpp
    ${SRC}/MCTS.cpp
    ${SRC}/game_state.hpp
)

target_link_libraries(mcts
    TreeSearch
    Games
)


