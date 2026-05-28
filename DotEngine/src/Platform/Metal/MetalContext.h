#pragma once

#include "DotEngine/Renderer/GraphicsContext.h"

namespace CA
{
    class MetalLayer;
}

namespace MTL
{
    class RenderPassDescriptor;
    class CommandQueue;
    class Device;
}

struct GLFWwindow;

namespace DotEngine
{
    class MetalContext : public GraphicsContext {

    public:
        explicit MetalContext(GLFWwindow* windowHandle);
        ~MetalContext() override;

        void Init() override;
        void SwapBuffers() override;
        [[nodiscard]] GraphicsAPI GetAPI() const override { return GraphicsAPI::Metal; }
        [[nodiscard]] void* GetDevice() const override { return m_Device; }
        [[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return m_MetalLayer; }
        [[nodiscard]] MTL::RenderPassDescriptor* GetRenderPassDescriptor() const { return m_RenderPassDescriptor; }
        [[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return m_CommandQueue; }
    private:
        void InitDevice();
        void InitWindow();
        void InitRenderPass();

        MTL::Device* m_Device;
        CA::MetalLayer* m_MetalLayer;
        MTL::CommandQueue* m_CommandQueue;
        MTL::RenderPassDescriptor* m_RenderPassDescriptor;
        GLFWwindow* m_WindowHandle;

    };
}
