cmake_minimum_required(VERSION 3.16)
project(DotEngine LANGUAGES CXX)

# Collect all source files
set(DOTENGINE_SOURCES
    # Precompiled header
        src/detpch.cpp
        src/detpch.h
    
    # Core engine files
        src/DotEngine.h
        src/DotEngine/Core.h
        src/DotEngine/Application.cpp
        src/DotEngine/Application.h
        src/DotEngine/EntryPoint.h
        src/DotEngine/Input.h
        src/DotEngine/KeyCodes.h
        src/DotEngine/MouseButtonCodes.h
        src/DotEngine/Layer.cpp
        src/DotEngine/Layer.h
        src/DotEngine/LayerStack.cpp
        src/DotEngine/LayerStack.h
        src/DotEngine/Log.cpp
        src/DotEngine/Log.h
        src/DotEngine/Window.h
    
    # Events
        src/DotEngine/Events/Event.h
        src/DotEngine/Events/ApplicationEvent.h
        src/DotEngine/Events/KeyEvent.h
        src/DotEngine/Events/MouseEvent.h
    
    # ImGui integration
        src/DotEngine/ImGui/ImGuiLayer.cpp
        src/DotEngine/ImGui/ImGuiLayer.h
    
    # Renderer
        src/DotEngine/Renderer/Buffer.cpp
        src/DotEngine/Renderer/Buffer.h
        src/DotEngine/Renderer/GraphicsContext.h
        src/DotEngine/Renderer/Renderer.cpp
        src/DotEngine/Renderer/Renderer.h
        src/DotEngine/Renderer/Shader.cpp
        src/DotEngine/Renderer/Shader.h
    
    # Platform - OpenGL
    src/Platform/OpenGL/OpenGLBuffer.cpp
    src/Platform/OpenGL/OpenGLBuffer.h
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLContext.h

    # Platform - Metal
    src/Platform/Metal/MetalContext.cpp
    src/Platform/Metal/MetalContext.h
)

# Platform-specific sources
if(WIN32)
    list(APPEND DOTENGINE_SOURCES
        src/Platform/Windows/WindowsInput.cpp
        src/Platform/Windows/WindowsInput.h
        src/Platform/Windows/WindowsWindow.cpp
        src/Platform/Windows/WindowsWindow.h
    )
elseif(APPLE)
    # On macOS, we'll use the Windows implementation for now
    # You may want to create macOS-specific implementations later
    list(APPEND DOTENGINE_SOURCES
        src/Platform/Mac/MacInput.cpp
        src/Platform/Mac/MacInput.h
        src/Platform/Mac/MacWindow.cpp
        src/Platform/Mac/MacWindow.h

        src/Platform/Metal/MetalImplementations.cpp
    )
elseif(UNIX)
    # On Linux, we'll use the Windows implementation for now
    list(APPEND DOTENGINE_SOURCES
        src/Platform/Windows/WindowsInput.cpp
        src/Platform/Windows/WindowsInput.h
        src/Platform/Windows/WindowsWindow.cpp
        src/Platform/Windows/WindowsWindow.h
    )
endif()

# Create static library
add_library(DotEngine STATIC ${DOTENGINE_SOURCES})

# Precompiled headers
target_precompile_headers(DotEngine PRIVATE src/detpch.h)

# Include directories
target_include_directories(DotEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw
)

# Link libraries
target_link_libraries(DotEngine PUBLIC
    glfw
    Glad
    ImGui
    glm::glm
    spdlog::spdlog
)

# Platform-specific definitions and libraries
if(WIN32)
    target_compile_definitions(DotEngine PUBLIC
        DOTENGINE_PLATFORM_WINDOWS
        GLFW_INCLUDE_NONE
    )

    # Link OpenGL on Windows
    target_link_libraries(DotEngine PUBLIC opengl32)

elseif(APPLE)
    target_compile_definitions(DotEngine PUBLIC
        DOTENGINE_PLATFORM_MACOS
        GLFW_INCLUDE_NONE
        GLFW_EXPOSE_NATIVE_COCOA
    )

    target_link_libraries(DotEngine PUBLIC MetalCPP)
elseif(UNIX)
    target_compile_definitions(DotEngine PUBLIC
        DOTENGINE_PLATFORM_LINUX
        GLFW_INCLUDE_NONE
    )

    # Link OpenGL on Linux
    find_package(OpenGL REQUIRED)

    target_link_libraries(DotEngine PUBLIC
        OpenGL::GL
        ${CMAKE_DL_LIBS}
    )
endif()

# Compiler-specific settings
if(MSVC)
    target_compile_options(DotEngine PRIVATE /W4)
else()
    target_compile_options(DotEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# C++ standard
target_compile_features(DotEngine PUBLIC cxx_std_23)
