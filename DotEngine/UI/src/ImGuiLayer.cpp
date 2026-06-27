#include "ImGuiLayer.h"
#include "EngineContext.h"
#include "GraphicsContext.h"
#include "Log.h"
#include "CoreTypes.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

#ifdef DOTENGINE_PLATFORM_WINDOWS
    #include "backends/imgui_impl_opengl3.h"
#else
    #include "MetalContext.h"
    #include "imgui_impl_metal.h"
    #include "QuartzCore/CAMetalDrawable.hpp"
    #include "MTLCommandBuffer.hpp"
    #include "MTLCommandQueue.hpp"
    #include "MTLRenderCommandEncoder.hpp"
    #include "CAMetalLayer.hpp"
#endif

#include <GLFW/glfw3.h>

namespace DotEngine {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding            = 4.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        style.ChildRounding    = 4.0f;
        style.FrameRounding    = 4.0f;
        style.PopupRounding    = 4.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding     = 3.0f;
        style.TabRounding      = 5.0f;

        DOTENGINE_CORE_ASSERT(m_Ctx, "ImGuiLayer requires a valid EngineContext!");

        auto* window = static_cast<GLFWwindow*>(m_Ctx->window.nativeHandle);

#ifdef DOTENGINE_PLATFORM_WINDOWS
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
#else
        ImGui_ImplGlfw_InitForOther(window, true);
        auto* device = static_cast<MTL::Device*>(m_Ctx->graphics.metal.device);
        DOTENGINE_CORE_ASSERT(device, "Metal device is null!");
        ImGui_ImplMetal_Init(device);
#endif
    }

    void ImGuiLayer::OnDetach() {
#ifdef DOTENGINE_PLATFORM_WINDOWS
        ImGui_ImplOpenGL3_Shutdown();
#else
        ImGui_ImplMetal_Shutdown();
#endif
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender() {
        static bool show = false;
        ImGui::ShowDemoWindow(&show);
    }

    void ImGuiLayer::Begin() {
        DOTENGINE_CORE_ASSERT(m_Ctx, "ImGuiLayer::Begin called without a valid EngineContext!");

#ifdef DOTENGINE_PLATFORM_WINDOWS
        ImGui_ImplOpenGL3_NewFrame();
#else
        auto* metalLayer = static_cast<CA::MetalLayer*>(m_Ctx->graphics.metal.metalLayer);
        auto* commandQueue = static_cast<MTL::CommandQueue*>(m_Ctx->graphics.metal.commandQueue);
        auto* rpd = static_cast<MTL::RenderPassDescriptor*>(m_Ctx->graphics.metal.renderPassDescriptor);

        auto* window = static_cast<GLFWwindow*>(m_Ctx->window.nativeHandle);
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        metalLayer->setDrawableSize(CGSizeMake(fbWidth, fbHeight));

        CA::MetalDrawable* drawable = metalLayer->nextDrawable();
        if (!drawable) return;

        rpd->colorAttachments()->object(0)->setTexture(drawable->texture());
        rpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        rpd->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
        rpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 1.0));

        m_Drawable     = drawable;
        m_CommandBuffer = commandQueue->commandBuffer();
        m_RenderCommandEncoder = m_CommandBuffer->renderCommandEncoder(rpd);
        m_RenderCommandEncoder->pushDebugGroup(
            NS::String::string("ImGui", NS::StringEncoding::ASCIIStringEncoding));

        ImGui_ImplMetal_NewFrame(rpd);
#endif

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        DOTENGINE_CORE_ASSERT(m_Ctx, "ImGuiLayer::End called without a valid EngineContext!");

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(
            static_cast<float>(m_Ctx->window.width),
            static_cast<float>(m_Ctx->window.height)
        );

        auto* window = static_cast<GLFWwindow*>(m_Ctx->window.nativeHandle);
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        if (fbWidth > 0 && fbHeight > 0) {
            io.DisplayFramebufferScale = ImVec2(
                static_cast<float>(fbWidth)  / static_cast<float>(m_Ctx->window.width),
                static_cast<float>(fbHeight) / static_cast<float>(m_Ctx->window.height)
            );
        }

        ImGui::Render();

#ifdef DOTENGINE_PLATFORM_WINDOWS
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
        if (m_RenderCommandEncoder && m_CommandBuffer && m_Drawable) {
            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffer, m_RenderCommandEncoder);
            m_RenderCommandEncoder->popDebugGroup();
            m_RenderCommandEncoder->endEncoding();
            m_CommandBuffer->presentDrawable(m_Drawable);
            m_CommandBuffer->commit();
            m_RenderCommandEncoder = nullptr;
            m_CommandBuffer        = nullptr;
            m_Drawable             = nullptr;
        }
#endif

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }

}
