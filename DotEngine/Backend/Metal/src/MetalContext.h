#pragma once

namespace DotEngine {
    struct GraphicsContext;
}

namespace DotEngine::Backend::Metal {

    void Init(GraphicsContext& ctx, void* glfwWindowHandle);
    void Shutdown(GraphicsContext& ctx);

}
