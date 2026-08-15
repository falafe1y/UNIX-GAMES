cmake_minimum_required(VERSION 3.20)

project(FGames
    VERSION 0.0.1
    DESCRIPTION "Classic games for UNIX and Windows in terminal"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

find_package(ftxui REQUIRED COMPONENTS
    screen
    dom
    component
)

# --------------------------------------------------
# Platform
# --------------------------------------------------

if(WIN32)
    set(PLATFORM_SOURCES
        core/platform/win/src/win_input.cpp
        core/platform/win/src/win_terminal.cpp
    )

elseif(UNIX)
    set(PLATFORM_SOURCES
        core/platform/linux/src/linux_input.cpp
        core/platform/linux/src/linux_terminal.cpp
    )

else()
    message(FATAL_ERROR "Unsupported platform")

endif()

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(FGames

    launcher/main.cpp
    launcher/launcher.cpp

    core/src/engine.cpp
    core/src/renderer.cpp
    core/src/timer.cpp

    games/snake/snake.cpp
    games/tetris/tetris.cpp
    games/demo/demo.cpp
    games/2048/2048.cpp

    ${PLATFORM_SOURCES}
)

# --------------------------------------------------
# Platform definition
# --------------------------------------------------

if(WIN32)
    target_compile_definitions(
        FGames
        PRIVATE
        FGames_PLATFORM_WINDOWS
    )

elseif(UNIX)
    target_compile_definitions(
        FGames
        PRIVATE
        FGames_PLATFORM_LINUX
    )

endif()

# --------------------------------------------------
# Include directories
# --------------------------------------------------

target_include_directories(FGames PRIVATE
    core/head
    
    core/platform/linux/head
    core/platform/win/head

    games/demo
    games/snake
    launcher
)

# --------------------------------------------------
# FTXUI
# --------------------------------------------------

target_link_libraries(FGames PRIVATE

    ftxui::screen
    ftxui::dom
    ftxui::component

)
