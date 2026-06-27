#pragma once

#include "RenderTypes.h"

namespace DotEngine::Renderer {

    // Implementations provided by the linked Backend module (OpenGL/Metal)
    VertexBuffer CreateVertexBuffer(const float* data, uint32_t size, const BufferLayout& layout);
    IndexBuffer  CreateIndexBuffer(const uint32_t* data, uint32_t count);
    void         BindVertexBuffer(const VertexBuffer& vb);
    void         BindIndexBuffer(const IndexBuffer& ib);
    void         DestroyVertexBuffer(VertexBuffer& vb);
    void         DestroyIndexBuffer(IndexBuffer& ib);

}
