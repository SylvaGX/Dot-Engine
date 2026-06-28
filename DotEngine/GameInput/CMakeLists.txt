cmake_minimum_required(VERSION 3.16)
project(DotGameInput LANGUAGES CXX)

set(GAMEINPUT_SOURCES
    src/GameInputTypes.h
    src/GameInputSystem.h
    src/GameInputSystem.cpp
)

add_library(DotGameInput STATIC ${GAMEINPUT_SOURCES})

target_include_directories(DotGameInput PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotGameInput PUBLIC
    DotEngineCore
    glfw
)

target_compile_features(DotGameInput PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotGameInput PRIVATE /W4)
else()
    target_compile_options(DotGameInput PRIVATE -Wall -Wextra -Wpedantic)
endif()
