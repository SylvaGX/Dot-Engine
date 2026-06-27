#pragma once

namespace DotEngine {

    enum class GraphicsAPI { None, OpenGL, Metal, DX12 };

    // Opaque platform handles — cast to real types inside the Backend modules only
    struct OpenGLContextData {
        void* windowHandle = nullptr; // GLFWwindow*
    };

    struct MetalContextData {
        void* device               = nullptr; // MTL::Device*
        void* metalLayer           = nullptr; // CA::MetalLayer*
        void* commandQueue         = nullptr; // MTL::CommandQueue*
        void* renderPassDescriptor = nullptr; // MTL::RenderPassDescriptor*
        void* windowHandle         = nullptr; // GLFWwindow*
    };

    struct GraphicsContext {
        GraphicsAPI api = GraphicsAPI::None;

        union {
            OpenGLContextData opengl;
            MetalContextData  metal;
            void*             none{nullptr};
        };

        GraphicsContext() : none{} {}
    };

}
