cmake_minimum_required(VERSION 3.16)
project(DotRenderer LANGUAGES CXX)

# Header-only API — implementations are provided by the linked Backend module
set(RENDERER_SOURCES
    src/RenderTypes.h
    src/BufferSystem.h
    src/ShaderSystem.h
    src/RenderSystem.h
)

add_library(DotRenderer INTERFACE)

target_include_directories(DotRenderer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotRenderer INTERFACE DotEngineCore)
