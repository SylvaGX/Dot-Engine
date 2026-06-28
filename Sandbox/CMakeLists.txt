cmake_minimum_required(VERSION 3.16)
project(Sandbox LANGUAGES CXX)

set(SANDBOX_SOURCES
    src/SandBoxApp.cpp
    src/editor/ExamplePanel.cpp
    src/editor/DockSpacePanel.cpp
    src/editor/DemoPanel.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Sandbox PRIVATE DotApplication)

if(WIN32 OR (UNIX AND NOT APPLE))
    target_link_libraries(Sandbox PRIVATE DotPlatformWindows)
elseif(APPLE)
    target_link_libraries(Sandbox PRIVATE DotPlatformMac)
endif()

target_compile_features(Sandbox PRIVATE cxx_std_23)

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
