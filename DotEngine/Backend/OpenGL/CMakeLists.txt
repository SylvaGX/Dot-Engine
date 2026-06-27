cmake_minimum_required(VERSION 3.16)
project(DotBackendOpenGL LANGUAGES CXX)

set(OPENGL_SOURCES
    src/OpenGLContext.h
    src/OpenGLContext.cpp
    src/OpenGLBuffer.cpp
    src/OpenGLShader.cpp
    src/OpenGLRender.cpp
)

add_library(DotBackendOpenGL STATIC ${OPENGL_SOURCES})

target_include_directories(DotBackendOpenGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotBackendOpenGL PUBLIC
    DotEngineCore
    DotRenderer
    Glad
    glfw
    opengl32
)

target_compile_definitions(DotBackendOpenGL PUBLIC GLFW_INCLUDE_NONE)
target_compile_features(DotBackendOpenGL PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotBackendOpenGL PRIVATE /W4)
else()
    target_compile_options(DotBackendOpenGL PRIVATE -Wall -Wextra -Wpedantic)
endif()
