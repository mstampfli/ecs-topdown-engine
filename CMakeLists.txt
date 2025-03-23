cmake_minimum_required(VERSION 3.16)
project(GameEngine)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Paths
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/src/headers)
set(LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/libraries)

# ✅ Collect all engine sources, including GraphicsLib
file(GLOB_RECURSE ENGINE_SOURCES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE GRAPHICSLIB_SOURCES ${LIBRARIES_DIR}/graphicslib/src/*.cpp)  # ✅ Add GraphicsLib sources

# ✅ Create the static library "gameengine" (libgameengine.a)
add_library(gameengine STATIC ${ENGINE_SOURCES} ${GRAPHICSLIB_SOURCES})  # ✅ GraphicsLib is now part of the engine!

# ✅ Ensure CMake looks for headers in the right places
target_include_directories(gameengine PRIVATE
  ${HEADERS_DIR}                         # Engine internal headers
  ${LIBRARIES_DIR}/graphicslib/include   # ✅ GraphicsLib headers
  ${LIBRARIES_DIR}/soloud/include        # ✅ SoLoud headers
  ${LIBRARIES_DIR}/glfw/include          # ✅ GLFW headers
  ${LIBRARIES_DIR}/glad/include          # ✅ GLAD headers
)

# ✅ Ensure the static library is built into the root build/ folder
set_target_properties(gameengine PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}  # ✅ Places libgameengine.a in the root build/ folder
)

message(STATUS "✅ GameEngine library (libgameengine.a) successfully built with GraphicsLib in build/ folder!")
