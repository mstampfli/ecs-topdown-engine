cmake_minimum_required(VERSION 3.16)
project(GameEngine)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Paths
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/src/headers)
set(LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/libraries)

# Collect engine sources but remove main.cpp (we want main.cpp compiled only in the executable)
file(GLOB_RECURSE ENGINE_SOURCES ${SRC_DIR}/*.cpp)
list(REMOVE_ITEM ENGINE_SOURCES ${SRC_DIR}/main.cpp)

# SoLoud sources
file(GLOB_RECURSE SOLOUD_BASE_SRC ${LIBRARIES_DIR}/soloud/src/core/*.cpp)
file(GLOB_RECURSE SOLOUD_FILTER_SRC ${LIBRARIES_DIR}/soloud/src/filter/*.cpp)

if(WIN32)
  file(GLOB_RECURSE SOLOUD_PLATFORM_SRC ${LIBRARIES_DIR}/soloud/src/backend/wasapi/*.cpp)
  add_compile_definitions(WITH_WASAPI)
elseif(APPLE)
  file(GLOB_RECURSE SOLOUD_PLATFORM_SRC ${LIBRARIES_DIR}/soloud/src/backend/coreaudio/*.cpp)
  add_compile_definitions(WITH_COREAUDIO)
elseif(UNIX AND NOT APPLE)
  file(GLOB_RECURSE SOLOUD_PLATFORM_SRC ${LIBRARIES_DIR}/soloud/src/backend/alsa/*.cpp)
  add_compile_definitions(WITH_ALSA)
else()
  file(GLOB_RECURSE SOLOUD_PLATFORM_SRC ${LIBRARIES_DIR}/soloud/src/backend/null/*.cpp)
  add_compile_definitions(WITH_NULLDRIVER)
endif()

set(SOLOUD_SRC
  ${SOLOUD_BASE_SRC}
  ${SOLOUD_FILTER_SRC}
  ${SOLOUD_PLATFORM_SRC}
)

# GraphicsLib sources
file(GLOB_RECURSE GRAPHICSLIB_SRC ${LIBRARIES_DIR}/graphicslib/src/*.cpp)

# Create the static library "gameengine" (this becomes libgameengine.a)
add_library(gameengine STATIC
  ${ENGINE_SOURCES}
  ${SOLOUD_SRC}
  ${GRAPHICSLIB_SRC}
)

# Include directories for gameengine
target_include_directories(gameengine PRIVATE
  ${HEADERS_DIR}                         # Engine internal headers
  ${LIBRARIES_DIR}/graphicslib/include   # GraphicsLib headers
  ${LIBRARIES_DIR}/soloud/include        # SoLoud headers
  ${LIBRARIES_DIR}/glfw/include          # GLFW headers (needed for linking)
  ${LIBRARIES_DIR}/glad/include          # GLAD headers (needed for linking)
)

# Build the executable (main.cpp is compiled here only)
set(MAIN_SRC ${SRC_DIR}/main.cpp)
add_executable(GameEngineExecutable ${MAIN_SRC})

# Ensure the linker finds GLFW and GLAD
target_link_directories(GameEngineExecutable PRIVATE
  ${LIBRARIES_DIR}/glfw/lib
  ${LIBRARIES_DIR}/glad/lib
)

# Link GLFW and GLAD to the executable, not the static engine library
target_link_libraries(GameEngineExecutable PRIVATE gameengine glfw3 glad)

# Platform-specific linker flags
if(WIN32)
  target_link_libraries(GameEngineExecutable PRIVATE winmm gdi32)
elseif(APPLE)
  target_link_libraries(GameEngineExecutable PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework CoreAudio")
elseif(UNIX AND NOT APPLE)
  target_link_libraries(GameEngineExecutable PRIVATE pthread dl X11)
endif()
