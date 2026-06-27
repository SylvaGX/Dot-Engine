cmake_minimum_required(VERSION 3.16)
project(Sandbox LANGUAGES CXX)

set(SANDBOX_SOURCES
    src/SandBoxApp.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})

# DotApplication brings: DotEngineCore, DotRenderer, DotUI (and all their transitive deps)
target_link_libraries(Sandbox PRIVATE DotApplication)

# Platform module brings: the platform window + input + the correct backend (OpenGL or Metal)
if(WIN32 OR (UNIX AND NOT APPLE))
    target_link_libraries(Sandbox PRIVATE DotPlatformWindows)
elseif(APPLE)
    target_link_libraries(Sandbox PRIVATE DotPlatformMac)
endif()

target_compile_features(Sandbox PRIVATE cxx_std_23)

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
