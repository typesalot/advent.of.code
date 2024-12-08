cmake_minimum_required(VERSION 3.14)

# Set the project name
project(aoc2024)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

# Add all .cpp files in the current directory to the executable
file(GLOB SOURCES "*.cpp" "*.h" "util/*.h" "util/*.cpp")

add_executable(aoc2024 ${SOURCES})
target_link_libraries(aoc2024 gtest_main)