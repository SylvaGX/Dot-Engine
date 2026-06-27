#include "BufferSystem.h"
#include "RenderTypes.h"

// Metal buffer management stubs — GPU buffers on Metal use MTL::Buffer.
// Full implementation would store MTL::Buffer* handles inside a separate registry.
// These stubs satisfy the linker; expand when Metal rendering is fully implemented.

namespace DotEngine::Renderer {

    VertexBuffer CreateVertexBuffer(const float* /*data*/, uint32_t /*size*/, const BufferLayout& layout) {
        VertexBuffer vb;
        vb.layout = layout;
        return vb;
    }

    IndexBuffer CreateIndexBuffer(const uint32_t* /*data*/, uint32_t count) {
        IndexBuffer ib;
        ib.count = count;
        return ib;
    }

    void BindVertexBuffer(const VertexBuffer& /*vb*/) {}
    void BindIndexBuffer(const IndexBuffer& /*ib*/)   {}
    void DestroyVertexBuffer(VertexBuffer& /*vb*/)    {}
    void DestroyIndexBuffer(IndexBuffer& /*ib*/)      {}

}
