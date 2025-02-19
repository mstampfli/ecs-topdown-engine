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

target_include_directories(MyRPGEngine PRIVATE 
    ${CMAKE_SOURCE_DIR}/graphicslib/src
    ${CMAKE_SOURCE_DIR}/graphicslib/libraries/include
    ${CMAKE_SOURCE_DIR}/graphicslib/shaders
)

link_directories(${CMAKE_SOURCE_DIR}/graphicslib/libraries/lib)
link_directories(${CMAKE_SOURCE_DIR}/graphicslib/build)

set(GRAPHICSLIB_A "${CMAKE_SOURCE_DIR}/graphicslib/build/libgraphicslib.a")
set(GLFW_A "${CMAKE_SOURCE_DIR}/graphicslib/libraries/lib/libglfw3.a")
set(GLAD_A "${CMAKE_SOURCE_DIR}/graphicslib/libraries/lib/libglad.a")

# ✅ Link with prebuilt `libgraphicslib.a`, `glfw3.a`, and `glad.a`
target_link_libraries(MyRPGEngine PRIVATE 
    ${GRAPHICSLIB_A}  # Directly link graphicslib.a
    ${GLFW_A}         # Link glfw3.a with full path
    ${GLAD_A}         # Link glad.a with full path
)

# Set output binary to build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Link Windows-specific libraries (if on Windows)
if (WIN32)
    target_link_libraries(MyRPGEngine PRIVATE winmm gdi32)
endif()
