cmake_minimum_required(VERSION 3.10)

# Project name
project(MyRPGEngine)

# Set C++ standard to 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define source directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/src/headers)

# Collect all .cpp files from src/
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

# Explicitly define main source file
set(MAIN_FILE ${SRC_DIR}/main.cpp)

# Define the executable (ensuring main.cpp is compiled)
add_executable(MyRPGEngine ${MAIN_FILE} ${SOURCES})

# Ensure headers are included
include_directories(${HEADERS_DIR})

# Set output binary to build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Link Windows-specific libraries (if on Windows)
if (WIN32)
    target_link_libraries(MyRPGEngine PRIVATE winmm gdi32)
endif()
