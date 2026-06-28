cmake_minimum_required(VERSION 3.16)
project(DotEngineCore LANGUAGES CXX)

set(ENGINECORE_SOURCES
    src/CoreMacros.h
    src/Log.h
    src/Log.cpp
    src/KeyCodes.h
    src/MouseButtonCodes.h
    src/WindowTypes.h
    src/GraphicsTypes.h
    src/EngineContext.h
    src/Platform.h
    src/Events/EventTypes.h
    src/Events/EventQueue.h
    src/Events/EventSystem.h
    src/Events/EventSystem.cpp
)

add_library(DotEngineCore STATIC ${ENGINECORE_SOURCES})

target_include_directories(DotEngineCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotEngineCore PUBLIC
    spdlog::spdlog
    glm::glm
    DotData
)

target_compile_features(DotEngineCore PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotEngineCore PRIVATE /W4)
else()
    target_compile_options(DotEngineCore PRIVATE -Wall -Wextra -Wpedantic)
endif()
