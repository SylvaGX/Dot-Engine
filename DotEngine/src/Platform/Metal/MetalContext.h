#pragma once

#include "DotEngine/Renderer/GraphicsContext.h"

#include "MTLDevice.hpp"
#include "MTLRenderPass.hpp"
#include "QuartzCore/CAMetalLayer.hpp"

struct GLFWwindow;

namespace DotEngine
{
    class MetalContext : public GraphicsContext {

    public:
        explicit MetalContext(GLFWwindow* windowHandle);
        ~MetalContext() override;

        void Init() override;
        void SwapBuffers() override;
        GraphicsAPI GetAPI() const override { return GraphicsAPI::Metal; }
        void* GetDevice() const override { return m_Device; }
        CA::MetalLayer* GetMetalLayer() const { return m_MetalLayer; }
        MTL::RenderPassDescriptor* GetRenderPassDescriptor() const { return m_RenderPassDescriptor; }
        MTL::CommandQueue* GetCommandQueue() const { return m_CommandQueue; }
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
