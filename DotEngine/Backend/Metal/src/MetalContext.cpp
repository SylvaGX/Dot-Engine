#include "MetalContext.h"

#include "GraphicsContext.h"
#include "Log.h"
#include "CoreTypes.h"

#include <GLFW/glfw3.h>

#include "Metal.hpp"
#include "AppKit.hpp"
#include "MTLDevice.hpp"
#include "MTLRenderPass.hpp"
#include "QuartzCore/CAMetalLayer.hpp"

extern "C" id glfwGetCocoaWindow(GLFWwindow* window);

namespace DotEngine::Backend::Metal {

    void Init(GraphicsContext& ctx, void* glfwWindowHandle) {
        ctx.api                    = GraphicsAPI::Metal;
        ctx.metal.windowHandle     = glfwWindowHandle;

        auto* window = static_cast<GLFWwindow*>(glfwWindowHandle);

        // Device
        MTL::Device* device = MTL::CreateSystemDefaultDevice();
        DOTENGINE_CORE_ASSERT(device, "Failed to create Metal device!");
        ctx.metal.device = device;

        DOTENGINE_CORE_INFO("Metal Info:");
        DOTENGINE_CORE_INFO("  Vendor: Apple");
        DOTENGINE_CORE_INFO("  Renderer: {0}", device->name()->utf8String());
        if (const MTL::Architecture* arch = device->architecture())
            DOTENGINE_CORE_INFO("  Architecture: {0}", arch->name()->utf8String());

        const char* metalVersion = "Metal 2";
        if (device->supportsFamily(MTL::GPUFamilyMetal4))      metalVersion = "Metal 4";
        else if (device->supportsFamily(MTL::GPUFamilyMetal3)) metalVersion = "Metal 3";
        DOTENGINE_CORE_INFO("  Version: {0}", metalVersion);

        // MetalLayer
        auto* nsWindow = static_cast<NS::Window*>(glfwGetCocoaWindow(window));
        NS::View* glfwView = nsWindow->contentView();

        CA::MetalLayer* metalLayer = CA::MetalLayer::layer();
        metalLayer->setDevice(device);
        metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        metalLayer->setFramebufferOnly(true);

        glfwView->setWantsLayer(true);
        glfwView->setLayer(metalLayer);
        ctx.metal.metalLayer = metalLayer;

        // Command queue
        MTL::CommandQueue* commandQueue = device->newCommandQueue();
        DOTENGINE_CORE_ASSERT(commandQueue, "Failed to create Metal command queue!");
        ctx.metal.commandQueue = commandQueue;

        // Render pass descriptor
        MTL::RenderPassDescriptor* rpd = MTL::RenderPassDescriptor::alloc()->init();
        DOTENGINE_CORE_ASSERT(rpd, "Failed to create Metal render pass descriptor!");
        ctx.metal.renderPassDescriptor = rpd;
    }

    void Shutdown(GraphicsContext& ctx) {
        if (ctx.metal.renderPassDescriptor)
            static_cast<MTL::RenderPassDescriptor*>(ctx.metal.renderPassDescriptor)->release();
        if (ctx.metal.commandQueue)
            static_cast<MTL::CommandQueue*>(ctx.metal.commandQueue)->release();
        if (ctx.metal.metalLayer)
            static_cast<CA::MetalLayer*>(ctx.metal.metalLayer)->release();
        if (ctx.metal.device)
            static_cast<MTL::Device*>(ctx.metal.device)->release();
        ctx.metal = {};
    }

    void* GetMetalLayer(const GraphicsContext& ctx)           { return ctx.metal.metalLayer; }
    void* GetCommandQueue(const GraphicsContext& ctx)         { return ctx.metal.commandQueue; }
    void* GetRenderPassDescriptor(const GraphicsContext& ctx) { return ctx.metal.renderPassDescriptor; }

}
