#pragma once

namespace DotEngine {
    struct GraphicsContext;
}

namespace DotEngine::Backend::Metal {

    void Init(GraphicsContext& ctx, void* glfwWindowHandle);
    void Shutdown(GraphicsContext& ctx);

    // Accessors used by the UI module — return typed pointers cast from void*
    void* GetMetalLayer(const GraphicsContext& ctx);
    void* GetCommandQueue(const GraphicsContext& ctx);
    void* GetRenderPassDescriptor(const GraphicsContext& ctx);

}
