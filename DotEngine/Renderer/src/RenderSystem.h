#pragma once

#include "RenderTypes.h"

namespace DotEngine {
    struct EngineContext;
}

namespace DotEngine::Renderer {

    // Implementations provided by the linked Backend module (OpenGL/Metal)
    void Init(EngineContext& ctx);
    void Clear(float r, float g, float b, float a = 1.0f);
    void DrawIndexed(uint32_t vao, const IndexBuffer& ib, const Shader& sh);
    void Present(EngineContext& ctx);
    void Shutdown(EngineContext& ctx);

}
