#pragma once

namespace DotEngine {
    struct GraphicsContext;
}

namespace DotEngine::Backend::OpenGL {

    void Init(GraphicsContext& ctx, void* glfwWindowHandle);
    void SwapBuffers(GraphicsContext& ctx);
    void Shutdown(GraphicsContext& ctx);

}
