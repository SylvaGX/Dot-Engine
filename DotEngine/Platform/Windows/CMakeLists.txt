cmake_minimum_required(VERSION 3.16)
project(DotPlatformWindows LANGUAGES CXX)

set(PLATFORM_WIN_SOURCES
    src/WindowsWindow.cpp
    src/WindowsInput.cpp
)

add_library(DotPlatformWindows STATIC ${PLATFORM_WIN_SOURCES})

target_include_directories(DotPlatformWindows PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotPlatformWindows PUBLIC
    DotEngineCore
    DotBackendOpenGL   # provides Renderer:: and Backend::OpenGL:: implementations
)

target_compile_features(DotPlatformWindows PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotPlatformWindows PRIVATE /W4)
else()
    target_compile_options(DotPlatformWindows PRIVATE -Wall -Wextra -Wpedantic)
endif()
