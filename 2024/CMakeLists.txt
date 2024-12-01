cmake_minimum_required(VERSION 3.10)

# Set the project name
project(aoc2024)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all .cpp files in the current directory to the executable
file(GLOB SOURCES "*.cpp"s "*.h")

add_executable(aoc2024 ${SOURCES})