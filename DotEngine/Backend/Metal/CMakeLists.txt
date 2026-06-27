cmake_minimum_required(VERSION 3.16)
project(DotBackendMetal LANGUAGES CXX)

set(METAL_SOURCES
    src/MetalContext.h
    src/MetalContext.cpp
    src/MetalBuffer.cpp
    src/MetalShader.cpp
    src/MetalRender.cpp
    src/MetalImplementations.cpp
)

add_library(DotBackendMetal STATIC ${METAL_SOURCES})

target_include_directories(DotBackendMetal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotBackendMetal PUBLIC
    DotEngineCore
    DotRenderer
    MetalCPP
    glfw
)

target_compile_definitions(DotBackendMetal PUBLIC
    GLFW_INCLUDE_NONE
    GLFW_EXPOSE_NATIVE_COCOA
)

target_compile_features(DotBackendMetal PUBLIC cxx_std_23)
