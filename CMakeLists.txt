cmake_minimum_required(VERSION 3.20)

project(TerminalGames)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    FGames

    launcher/main.cpp
    launcher/launcher.cpp
    
    core/src/engine.cpp
    core/src/input.cpp

    games/demo/demo.cpp
)