#include "BufferSystem.h"
#include "RenderTypes.h"
#include "Log.h"
#include "CoreTypes.h"

#include <glad/glad.h>

namespace DotEngine::Renderer {

    static GLenum ToGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
            default:
                DOTENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
        }
    }

    VertexBuffer CreateVertexBuffer(const float* data, uint32_t size, const BufferLayout& layout) {
        VertexBuffer vb;
        vb.layout = layout;

        glCreateBuffers(1, &vb.rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, vb.rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        return vb;
    }

    IndexBuffer CreateIndexBuffer(const uint32_t* data, uint32_t count) {
        IndexBuffer ib;
        ib.count = count;

        glCreateBuffers(1, &ib.rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);

        return ib;
    }

    void BindVertexBuffer(const VertexBuffer& vb) {
        glBindBuffer(GL_ARRAY_BUFFER, vb.rendererID);

        uint32_t index = 0;
        for (const auto& element : vb.layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                static_cast<int>(element.GetComponentCount()),
                ToGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                static_cast<int>(vb.layout.GetStride()),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset))
            );
            index++;
        }
    }

    void BindIndexBuffer(const IndexBuffer& ib) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.rendererID);
    }

    void DestroyVertexBuffer(VertexBuffer& vb) {
        glDeleteBuffers(1, &vb.rendererID);
        vb.rendererID = 0;
    }

    void DestroyIndexBuffer(IndexBuffer& ib) {
        glDeleteBuffers(1, &ib.rendererID);
        ib.rendererID = 0;
        ib.count      = 0;
    }

}
