#include "EditorSystem.h"

#include "EngineContext.h"
#include "EditorContext.h"
#include "GraphicsTypes.h"
#include "Log.h"
#include "CoreMacros.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

#if defined(DOTENGINE_PLATFORM_MACOS)
    #include "imgui_impl_metal.h"
    #include "QuartzCore/CAMetalDrawable.hpp"
    #include "MTLCommandBuffer.hpp"
    #include "MTLCommandQueue.hpp"
    #include "MTLRenderCommandEncoder.hpp"
    #include "CAMetalLayer.hpp"
#else
    #include "backends/imgui_impl_opengl3.h"
#endif

#include <GLFW/glfw3.h>

namespace DotEngine::Editor {

    void Init(EngineContext& ctx, EditorContext& editor) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding              = 4.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        style.ChildRounding     = 4.0f;
        style.FrameRounding     = 4.0f;
        style.PopupRounding     = 4.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding      = 3.0f;
        style.TabRounding       = 5.0f;

        auto* window = static_cast<GLFWwindow*>(ctx.window.nativeHandle);
        DOTENGINE_CORE_ASSERT(window, "Editor::Init requires a valid native window handle!");

#if defined(DOTENGINE_PLATFORM_MACOS)
        ImGui_ImplGlfw_InitForOther(window, true);
        auto* device = static_cast<MTL::Device*>(ctx.graphics.metal.device);
        DOTENGINE_CORE_ASSERT(device, "Metal device is null!");
        ImGui_ImplMetal_Init(device);
#else
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
#endif

        editor.attached = true;
    }

    void Shutdown(EditorContext& editor) {
        if (!editor.attached)
            return;

#if defined(DOTENGINE_PLATFORM_MACOS)
        ImGui_ImplMetal_Shutdown();
#else
        ImGui_ImplOpenGL3_Shutdown();
#endif
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        editor = {};
    }

    void UpdateAll(EngineContext& ctx, EditorContext& editor) {
        for (auto& entry : editor.panels.entries) {
            if (entry.onUpdate)
                entry.onUpdate(ctx, editor, entry.userData);
        }
    }

    void DrawPanels(EngineContext& ctx, EditorContext& editor) {
        if (!editor.frameActive)
            return;

        for (auto& entry : editor.panels.entries) {
            if (entry.onPanels)
                entry.onPanels(ctx, editor, entry.userData);
        }
    }

    void Begin(EngineContext& ctx, EditorContext& editor) {
        editor.frameActive = false;

#if defined(DOTENGINE_PLATFORM_MACOS)
        auto* metalLayer   = static_cast<CA::MetalLayer*>(ctx.graphics.metal.metalLayer);
        auto* commandQueue = static_cast<MTL::CommandQueue*>(ctx.graphics.metal.commandQueue);
        auto* rpd          = static_cast<MTL::RenderPassDescriptor*>(ctx.graphics.metal.renderPassDescriptor);

        auto* window = static_cast<GLFWwindow*>(ctx.window.nativeHandle);
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        metalLayer->setDrawableSize(CGSizeMake(fbWidth, fbHeight));

        CA::MetalDrawable* drawable = metalLayer->nextDrawable();
        if (!drawable)
            return;

        rpd->colorAttachments()->object(0)->setTexture(drawable->texture());
        rpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        rpd->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
        rpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 1.0));

        editor.drawable       = drawable;
        editor.commandBuffer  = commandQueue->commandBuffer();
        editor.renderEncoder  = static_cast<void*>(
            static_cast<MTL::CommandBuffer*>(editor.commandBuffer)->renderCommandEncoder(rpd));
        static_cast<MTL::RenderCommandEncoder*>(editor.renderEncoder)->pushDebugGroup(
            NS::String::string("ImGui", NS::StringEncoding::ASCIIStringEncoding));

        ImGui_ImplMetal_NewFrame(rpd);
#else
        ImGui_ImplOpenGL3_NewFrame();
#endif

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        editor.frameActive = true;
    }

    void End(EngineContext& ctx, EditorContext& editor) {
        if (!editor.frameActive)
            return;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(
            static_cast<float>(ctx.window.width),
            static_cast<float>(ctx.window.height)
        );

        auto* window = static_cast<GLFWwindow*>(ctx.window.nativeHandle);
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        if (fbWidth > 0 && fbHeight > 0) {
            io.DisplayFramebufferScale = ImVec2(
                static_cast<float>(fbWidth)  / static_cast<float>(ctx.window.width),
                static_cast<float>(fbHeight) / static_cast<float>(ctx.window.height)
            );
        }

        ImGui::Render();

#if defined(DOTENGINE_PLATFORM_MACOS)
        if (editor.renderEncoder && editor.commandBuffer && editor.drawable) {
            auto* encoder = static_cast<MTL::RenderCommandEncoder*>(editor.renderEncoder);
            auto* buffer  = static_cast<MTL::CommandBuffer*>(editor.commandBuffer);
            auto* drawable = static_cast<CA::MetalDrawable*>(editor.drawable);

            ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), buffer, encoder);
            encoder->popDebugGroup();
            encoder->endEncoding();
            buffer->presentDrawable(drawable);
            buffer->commit();
        }
        editor.renderEncoder = nullptr;
        editor.commandBuffer = nullptr;
        editor.drawable      = nullptr;
#else
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }

        editor.frameActive = false;
    }

}
