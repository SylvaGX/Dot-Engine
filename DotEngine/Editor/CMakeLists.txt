cmake_minimum_required(VERSION 3.16)
project(DotEditor LANGUAGES CXX)

set(EDITOR_SOURCES
    src/EditorContext.h
    src/EditorEventSystem.h
    src/EditorEventSystem.cpp
    src/EditorSystem.h
    src/EditorSystem.cpp
    src/EditorPanelSystem.h
    src/EditorPanelSystem.cpp
)

add_library(DotEditor STATIC ${EDITOR_SOURCES})

target_include_directories(DotEditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotEditor PUBLIC
    DotEngineCore
    DotRenderer
    ImGui
    glfw
)

if(APPLE)
    target_link_libraries(DotEditor PUBLIC MetalCPP)
    target_compile_definitions(DotEditor PUBLIC IMGUI_IMPL_METAL_CPP)
endif()

target_compile_features(DotEditor PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotEditor PRIVATE /W4)
else()
    target_compile_options(DotEditor PRIVATE -Wall -Wextra -Wpedantic)
endif()
