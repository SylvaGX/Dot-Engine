#include "RenderSystem.h"
#include "EngineContext.h"

// Metal has no standalone Clear/DrawIndexed outside a render pass.
// Those are driven by the Editor module's Begin/End.
// These stubs satisfy the linker when the Metal backend is selected.

namespace DotEngine::Renderer {

    void Init(EngineContext& /*ctx*/) {}

    void Clear(float /*r*/, float /*g*/, float /*b*/, float /*a*/) {
        // Metal clear colour is set on the render pass descriptor in Editor::Begin
    }

    void DrawIndexed(uint32_t /*vao*/, const IndexBuffer& /*ib*/, const Shader& /*sh*/) {
        // Metal draw calls are encoded into a render command encoder
    }

    void Present(EngineContext& /*ctx*/) {
        // Metal presentation is committed in Editor::End via command buffer
    }

    void Shutdown(EngineContext& /*ctx*/) {}

}
