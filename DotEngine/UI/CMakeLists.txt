cmake_minimum_required(VERSION 3.16)
project(DotUI LANGUAGES CXX)

set(UI_SOURCES
    src/ImGuiLayer.h
    src/ImGuiLayer.cpp
    src/UISystem.h
    src/UISystem.cpp
)

add_library(DotUI STATIC ${UI_SOURCES})

target_include_directories(DotUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotUI PUBLIC
    DotEngineCore
    DotRenderer
    ImGui
    glfw
)

if(APPLE)
    target_link_libraries(DotUI PUBLIC MetalCPP)
    target_compile_definitions(DotUI PUBLIC IMGUI_IMPL_METAL_CPP)
endif()

target_compile_features(DotUI PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotUI PRIVATE /W4)
else()
    target_compile_options(DotUI PRIVATE -Wall -Wextra -Wpedantic)
endif()
