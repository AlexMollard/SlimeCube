cmake_minimum_required(VERSION 3.16)

set(ENV{VCPKG_ROOT} "E:/vcpkg/")
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else ()
    message("Make sure you have set your VCPKG ENV VAR")
endif ()

project(SlimeCube CXX)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS_RELEASE "/ZI")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
find_package(assimp CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

# Set code directories
FILE(GLOB FILES_TO_BUILD src/*.h src/*.cpp)

# Compiling project
add_executable(SlimeCube ${FILES_TO_BUILD})

#include pch
target_precompile_headers(SlimeCube PUBLIC src/pch.h)

#Need to include the backend stuff because vcpkg doesn't wanna
set(imgui-backend-includes "${PROJECT_SOURCE_DIR}/Imgui-Backend/")
add_library(imgui STATIC ${imgui-backend-includes}/imgui_impl_glfw.cpp ${imgui-backend-includes}/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${imgui-backend-includes})
target_link_libraries(imgui PUBLIC imgui::imgui)

target_include_directories(SlimeCube PUBLIC ${imgui-backend-includes})

target_link_libraries(SlimeCube PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(SlimeCube PUBLIC glfw)
target_link_libraries(SlimeCube PUBLIC GLEW::GLEW)
target_link_libraries(SlimeCube PUBLIC imgui)
target_link_libraries(SlimeCube PUBLIC imguizmo::imguizmo)
target_link_libraries(SlimeCube PUBLIC spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(SlimeCube PUBLIC glm::glm)
target_link_libraries(SlimeCube PUBLIC EnTT::EnTT)
target_link_libraries(SlimeCube PRIVATE assimp::assimp)

target_include_directories(SlimeCube PUBLIC ${STB_INCLUDE_DIRS})