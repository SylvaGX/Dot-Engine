cmake_minimum_required(VERSION 3.16)
project(DotApplication LANGUAGES CXX)

set(APPLICATION_SOURCES
    src/ApplicationContext.h
    src/Application.h
    src/Application.cpp
    src/EntryPoint.h
    src/DotEngine.h
)

add_library(DotApplication STATIC ${APPLICATION_SOURCES})

target_include_directories(DotApplication PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotApplication PUBLIC
    DotData
    DotEngineCore
    DotRenderer
    DotGameInput
)

if(DOT_EDITOR)
    target_link_libraries(DotApplication PUBLIC DotEditor)
    target_compile_definitions(DotApplication PUBLIC DOT_EDITOR)
endif()

target_compile_features(DotApplication PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotApplication PRIVATE /W4)
else()
    target_compile_options(DotApplication PRIVATE -Wall -Wextra -Wpedantic)
endif()
