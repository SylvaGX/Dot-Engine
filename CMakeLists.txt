cmake_minimum_required(VERSION 3.16)
project(DotEngine VERSION 1.0.0 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Platform detection
if(WIN32)
    add_compile_definitions(DOTENGINE_PLATFORM_WINDOWS)
elseif(APPLE)
    add_compile_definitions(DOTENGINE_PLATFORM_MACOS)
elseif(UNIX)
    add_compile_definitions(DOTENGINE_PLATFORM_LINUX)
endif()

# Configuration-specific definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DOTENGINE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(DOTENGINE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(DOTENGINE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_definitions(DOTENGINE_DIST)
endif()

# Vendor dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(DotEngine/vendor/glfw)

# GLM
find_package(glm REQUIRED)

# spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
find_package(spdlog REQUIRED)

# Glad
add_subdirectory(DotEngine/vendor/Glad)

# ImGui (using wrapper CMakeLists to avoid modifying submodule)
add_subdirectory(DotEngine/vendor/imgui_cmake)

# Dot Engine
add_subdirectory(DotEngine)

# Sandbox Application
add_subdirectory(Sandbox)

# Set Sandbox as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
