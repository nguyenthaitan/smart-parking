cmake_minimum_required(VERSION 3.10)
project(smart_parking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(smart_parking ${SOURCES})

# Tests
enable_testing()
add_subdirectory(tests)
