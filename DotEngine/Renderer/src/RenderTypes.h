#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace DotEngine {

    enum class ShaderDataType {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    inline uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3:   return 4 * 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4 * 4;
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 4 * 2;
            case ShaderDataType::Int3:   return 4 * 3;
            case ShaderDataType::Int4:   return 4 * 4;
            case ShaderDataType::Bool:   return 1;
            default:                     return 0;
        }
    }

    inline uint32_t ShaderDataTypeComponentCount(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 9;
            case ShaderDataType::Mat4:   return 16;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            default:                     return 0;
        }
    }

    struct BufferElement {
        std::string    Name;
        ShaderDataType Type           = ShaderDataType::None;
        uint32_t       Size           = 0;
        uint32_t       Offset         = 0;
        uint32_t       componentCount = 0;
        bool           Normalized     = false;
    };

    struct BufferLayout {
        uint32_t                   stride = 0;
        std::vector<BufferElement> elements;
    };

    inline void BufferLayoutBuild(BufferLayout& layout) {
        uint32_t offset = 0;
        layout.stride   = 0;
        for (auto& element : layout.elements) {
            element.componentCount = ShaderDataTypeComponentCount(element.Type);
            element.Size           = ShaderDataTypeSize(element.Type);
            element.Offset         = offset;
            offset                += element.Size;
            layout.stride         += element.Size;
        }
    }

    inline BufferLayout MakeBufferLayout(std::initializer_list<BufferElement> elements) {
        BufferLayout layout;
        layout.elements.assign(elements);
        BufferLayoutBuild(layout);
        return layout;
    }

    struct VertexBuffer {
        uint32_t     rendererID = 0;
        BufferLayout layout;
    };

    struct IndexBuffer {
        uint32_t rendererID = 0;
        uint32_t count      = 0;
    };

    struct Shader {
        uint32_t rendererID = 0;
    };

}
