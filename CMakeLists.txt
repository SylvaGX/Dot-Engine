cmake_minimum_required(VERSION 3.16)
project(DotEngine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Platform definitions (propagated to all targets via add_compile_definitions)
if(WIN32)
    add_compile_definitions(DOTENGINE_PLATFORM_WINDOWS)
elseif(APPLE)
    add_compile_definitions(DOTENGINE_PLATFORM_MACOS IMGUI_IMPL_METAL_CPP)
elseif(UNIX)
    add_compile_definitions(DOTENGINE_PLATFORM_LINUX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DOTENGINE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(DOTENGINE_RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_definitions(DOTENGINE_DIST)
endif()

# ---------- Vendor dependencies ----------


# ------- GLFW -------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(DotEngine/vendor/glfw)
# --------------------

# ------- GLM -------
find_package(glm REQUIRED)
# -------------------

# ------- SPDLOG -------
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTING  OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL        OFF CACHE BOOL "" FORCE)
find_package(spdlog REQUIRED)
# ----------------------

# ------- NLOHMANN JSON -------
find_package(nlohmann_json REQUIRED)
# -----------------------------

# ------- JSON SCHEMA VALIDATOR -------
find_package(nlohmann_json_schema_validator REQUIRED)
# -------------------------------------

# ------- GLAD -------
add_subdirectory(DotEngine/vendor/Glad)
# --------------------

# -------- METALCPP -------
if(APPLE)
    add_subdirectory(DotEngine/vendor/metal-cpp)
endif()
# -------------------------

# ------- IMGUI -------
add_subdirectory(DotEngine/vendor/imgui_cmake)
# ---------------------


# ---------- Engine modules ----------

add_subdirectory(DotEngine/Data)
add_subdirectory(DotEngine/EngineCore)
add_subdirectory(DotEngine/Renderer)

if(WIN32 OR (UNIX AND NOT APPLE))
    add_subdirectory(DotEngine/Backend/OpenGL)
    add_subdirectory(DotEngine/Platform/Windows)
elseif(APPLE)
    add_subdirectory(DotEngine/Backend/Metal)
    add_subdirectory(DotEngine/Platform/Mac)
endif()

add_subdirectory(DotEngine/UI)
add_subdirectory(DotEngine/Application)

# ---------- Sandbox ----------

add_subdirectory(Sandbox)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
