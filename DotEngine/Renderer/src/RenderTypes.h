#pragma once

#include <cstdint>
#include <string>
#include <utility>
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

    struct BufferElement {
        std::string    Name;
        ShaderDataType Type       = ShaderDataType::None;
        uint32_t       Size       = 0;
        uint32_t       Offset     = 0;
        bool           Normalized = false;

        BufferElement() = default;
        BufferElement(const ShaderDataType type, std::string  name, const bool normalized = false)
            : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized) {}

        uint32_t GetComponentCount() const {
            switch (Type) {
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
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement> elements)
            : m_Elements(elements) { CalculateOffsetAndStride(); }

        uint32_t                          GetStride()   const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        auto begin()       { return m_Elements.begin(); }
        auto end()         { return m_Elements.end(); }
        auto begin() const { return m_Elements.begin(); }
        auto end()   const { return m_Elements.end(); }

    private:
        void CalculateOffsetAndStride() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& e : m_Elements) {
                e.Offset  = offset;
                offset   += e.Size;
                m_Stride += e.Size;
            }
        }
        std::vector<BufferElement> m_Elements;
        uint32_t                   m_Stride = 0;
    };

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
