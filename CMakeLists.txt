cmake_minimum_required(VERSION 3.21)
project(SlimeCube)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

#disable in lining for profiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(_CRT_FUNCTIONS_REQUIRED 0)

#Include ProjectFolders
include_directories(
        src
        Dependencies
        Dependencies/spdlog/include
        Dependencies/glm
        Dependencies/entt
        Dependencies/stb
        Dependencies/imgui
        Dependencies/imguizmo
)

# Set code directories
FILE(GLOB FILES_TO_BUILD
        src/*.h src/*.cpp
        Dependencies/imgui/*.h Dependencies/imgui/*.cpp
        Dependencies/imguizmo/*.h Dependencies/imguizmo/*.cpp
        )

# Compiling project
add_executable(SlimeCube ${FILES_TO_BUILD} src/SlimeMath.cpp)

find_package(OpenGL REQUIRED)
target_link_libraries(SlimeCube PRIVATE
        ${OPENGL_gl_LIBRARY}
        ${PROJECT_SOURCE_DIR}/Dependencies/glfw/glfw3.lib
        ${PROJECT_SOURCE_DIR}/Dependencies/glfw/glfw3dll.lib
        ${PROJECT_SOURCE_DIR}/Dependencies/glew/glew32.lib
        )

include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/glfw/include)

