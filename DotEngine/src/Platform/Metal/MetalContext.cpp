#include "detpch.h"

#include "MetalContext.h"

#include <GLFW/glfw3.h>

#include "Metal.hpp"
#include "AppKit.hpp"
#include "MTLDevice.hpp"
#include "MTLRenderPass.hpp"
#include "QuartzCore/CAMetalLayer.hpp"

extern "C" id glfwGetCocoaWindow(GLFWwindow* window);

namespace DotEngine
{
    MetalContext::MetalContext(GLFWwindow* windowHandle)
        : m_Device(nullptr), m_MetalLayer(nullptr), m_CommandQueue(nullptr),
          m_RenderPassDescriptor(nullptr), m_WindowHandle(windowHandle)
    {
        DOTENGINE_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    MetalContext::~MetalContext()
    {
        if (m_RenderPassDescriptor)
            m_RenderPassDescriptor->release();

        if (m_CommandQueue)
            m_CommandQueue->release();

        if (m_MetalLayer)
            m_MetalLayer->release();

        if (m_Device)
            m_Device->release();
    }

    void MetalContext::Init()
    {
        InitDevice();
        InitWindow();
        InitRenderPass();
    }

    void MetalContext::SwapBuffers()
    {
    }

    void MetalContext::InitDevice()
    {
        m_Device = MTL::CreateSystemDefaultDevice();
        DOTENGINE_CORE_ASSERT(m_Device, "Failed to create Metal device!");

        DOTENGINE_CORE_INFO("Metal Info:");
        DOTENGINE_CORE_INFO("  Vendor: Apple");
        DOTENGINE_CORE_INFO("  Renderer: {0}", m_Device->name()->utf8String());

        if (const MTL::Architecture* architecture = m_Device->architecture())
            DOTENGINE_CORE_INFO("  Architecture: {0}", architecture->name()->utf8String());

        const char* metalVersion = "Metal 2";
        if (m_Device->supportsFamily(MTL::GPUFamilyMetal4))
            metalVersion = "Metal 4";
        else if (m_Device->supportsFamily(MTL::GPUFamilyMetal3))
            metalVersion = "Metal 3";

        DOTENGINE_CORE_INFO("  Version: {0}", metalVersion);
    }

    void MetalContext::InitWindow()
    {
        const auto nsWindow = static_cast<NS::Window*>(glfwGetCocoaWindow(m_WindowHandle));
        NS::View* glfwView = nsWindow->contentView();

        m_MetalLayer = CA::MetalLayer::layer();
        m_MetalLayer->setDevice(m_Device);
        m_MetalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        m_MetalLayer->setFramebufferOnly(true);

        glfwView->setWantsLayer(true);
        glfwView->setLayer(m_MetalLayer);
    }

    void MetalContext::InitRenderPass()
    {
        m_CommandQueue = m_Device->newCommandQueue();
        DOTENGINE_CORE_ASSERT(m_CommandQueue, "Failed to create Metal command queue!");

        m_RenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        DOTENGINE_CORE_ASSERT(m_RenderPassDescriptor, "Failed to create Metal render pass descriptor!");
    }
}
