cmake_minimum_required(VERSION 3.10)

# Set the project name
project(chip8)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_executable(automaton src/main.cpp src/grid.cpp src/element.cpp
  src/utils.cpp src/renderer.cpp src/inputhandler.cpp)

find_package(SFML 2.6 COMPONENTS graphics audio window system REQUIRED)
target_link_libraries(automaton sfml-graphics sfml-audio sfml-window sfml-system)

# Enable profiling with gprof for the chip8 target
set_target_properties(automaton PROPERTIES
    CXX_STANDARD 20
    COMPILE_FLAGS "-Wall -p -pg"
    LINK_FLAGS "-p -pg"
)
