cmake_minimum_required(VERSION 3.16)
project(DotPlatformMac LANGUAGES CXX)

set(PLATFORM_MAC_SOURCES
    src/MacWindow.cpp
)

add_library(DotPlatformMac STATIC ${PLATFORM_MAC_SOURCES})

target_include_directories(DotPlatformMac PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotPlatformMac PUBLIC
    DotEngineCore
    DotBackendMetal
    DotApplication
)

target_compile_features(DotPlatformMac PUBLIC cxx_std_23)
