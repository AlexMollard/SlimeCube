cmake_minimum_required(VERSION 3.12)

set(ENV{VCPKG_ROOT} "C:/Libs/vcpkg")
if(DEFINED ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
  message("It broke")
endif()

project(SlimeCube CXX)

set (CMAKE_CXX_STANDARD 17)

set(_CRT_FUNCTIONS_REQUIRED 0)

#Include ProjectFolders
include_directories(src)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

# Set code directories
FILE(GLOB FILES_TO_BUILD src/*.h src/*.cpp)

# Compiling project
add_executable(SlimeCube ${FILES_TO_BUILD})

target_link_libraries(SlimeCube PUBLIC OpenGL)
target_link_libraries(SlimeCube PUBLIC glfw)
target_link_libraries(SlimeCube PUBLIC GLEW::GLEW)
target_link_libraries(SlimeCube PUBLIC imgui::imgui)
target_link_libraries(SlimeCube PUBLIC imguizmo::imguizmo)
target_link_libraries(SlimeCube PUBLIC spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(SlimeCube PUBLIC glm::glm)
target_link_libraries(SlimeCube PUBLIC EnTT::EnTT)
target_include_directories(SlimeCube PUBLIC ${STB_INCLUDE_DIRS})