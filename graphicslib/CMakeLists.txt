cmake_minimum_required(VERSION 3.10)
project(graphicslib)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ✅ Include directories (headers for GLAD, GLFW, and dependencies)
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libraries/include  # GLAD, GLFW, GLM, STB
    ${CMAKE_SOURCE_DIR}/shaders
)

# ✅ Tell CMake where to find the prebuilt libraries
link_directories(${CMAKE_SOURCE_DIR}/libraries/lib)

# ✅ Collect all .cpp files in src/
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# ✅ Create a static library
add_library(graphicslib STATIC ${SOURCES})

# ✅ Link external libraries (GLFW, OpenGL, prebuilt GLAD)
find_package(OpenGL REQUIRED)

target_link_libraries(graphicslib
    glfw3        # GLFW (from libraries/lib/)
    glad         # GLAD (from libraries/lib/)
)

# ✅ Ensure GDI is linked properly on Windows
if (WIN32)
    target_link_libraries(graphicslib gdi32)
endif()
