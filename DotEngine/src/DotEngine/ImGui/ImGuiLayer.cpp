#include "detpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"

#ifdef DOTENGINE_PLATFORM_WINDOWS
#include "backends/imgui_impl_opengl3.h"
#else
#include "Platform/Metal/MetalContext.h"
#include "imgui_impl_metal.h"
#include "QuartzCore/CAMetalDrawable.hpp"
#include "MTLCommandBuffer.hpp"
#include "MTLCommandQueue.hpp"
#include "MTLRenderCommandEncoder.hpp"
#include "CAMetalLayer.hpp"
#endif

#include "DotEngine/Application.h"

#include <GLFW/glfw3.h>

#include "DotEngine/Renderer/GraphicsContext.h"

namespace DotEngine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
#ifdef DOTENGINE_PLATFORM_MACOS
		, m_RenderCommandEncoder(nullptr)
		, m_CommandBuffer(nullptr)
		, m_Drawable(nullptr)
#endif
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const auto windowAPI = app.GetWindow().GetGraphicsContext()->GetAPI();

#ifndef DOTENGINE_PLATFORM_MACOS
		if (windowAPI == GraphicsAPI::OpenGL)
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
#else
		if (windowAPI == GraphicsAPI::Metal)
		{
			ImGui_ImplGlfw_InitForOther(window, true);

			auto* metalContext = static_cast<MetalContext*>(app.GetWindow().GetGraphicsContext());
			auto* device = static_cast<MTL::Device*>(metalContext->GetDevice());
			DOTENGINE_CORE_ASSERT(device, "Metal device is null!");
			ImGui_ImplMetal_Init(device);
		}
#endif
	}

	void ImGuiLayer::OnDetach()
	{
		Application& app = Application::Get();
		const auto windowAPI = app.GetWindow().GetGraphicsContext()->GetAPI();

#ifndef DOTENGINE_PLATFORM_MACOS
		if (windowAPI == GraphicsAPI::OpenGL)
			ImGui_ImplOpenGL3_Shutdown();
#else
		if (windowAPI == GraphicsAPI::Metal)
			ImGui_ImplMetal_Shutdown();
#endif

		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin()
	{
		auto& app = Application::Get();
		const auto windowAPI = app.GetWindow().GetGraphicsContext()->GetAPI();

#ifndef DOTENGINE_PLATFORM_MACOS
		if (windowAPI == GraphicsAPI::OpenGL)
			ImGui_ImplOpenGL3_NewFrame();
#else
		if (windowAPI == GraphicsAPI::Metal)
		{
			const auto* metalContext = dynamic_cast<MetalContext*>(app.GetWindow().GetGraphicsContext());
			const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

			int framebufferWidth = 0;
			int framebufferHeight = 0;
			glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

			CA::MetalLayer* metalLayer = metalContext->GetMetalLayer();
			metalLayer->setDrawableSize(CGSizeMake(framebufferWidth, framebufferHeight));

			CA::MetalDrawable* drawable = metalLayer->nextDrawable();
			if (!drawable)
				return;

			MTL::RenderPassDescriptor* renderPassDescriptor = metalContext->GetRenderPassDescriptor();
			renderPassDescriptor->colorAttachments()->object(0)->setTexture(drawable->texture());
			renderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
			renderPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
			renderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.1, 0.1, 0.1, 1.0));

			m_Drawable = drawable;

			m_CommandBuffer = metalContext->GetCommandQueue()->commandBuffer();
			m_RenderCommandEncoder = m_CommandBuffer->renderCommandEncoder(renderPassDescriptor);

			m_RenderCommandEncoder->pushDebugGroup(NS::String::string("ImGui", NS::StringEncoding::ASCIIStringEncoding));

			ImGui_ImplMetal_NewFrame(renderPassDescriptor);
		}
#endif

		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		const auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		int framebufferWidth = 0;
		int framebufferHeight = 0;
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));
		if (framebufferWidth > 0 && framebufferHeight > 0)
		{
			io.DisplayFramebufferScale = ImVec2(
				static_cast<float>(framebufferWidth) / static_cast<float>(app.GetWindow().GetWidth()),
				static_cast<float>(framebufferHeight) / static_cast<float>(app.GetWindow().GetHeight())
			);
		}

		ImGui::Render();

		const auto windowAPI = app.GetWindow().GetGraphicsContext()->GetAPI();

#ifndef DOTENGINE_PLATFORM_MACOS
		if (windowAPI == GraphicsAPI::OpenGL)
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
		if (windowAPI == GraphicsAPI::Metal)
		{
			if (m_RenderCommandEncoder && m_CommandBuffer && m_Drawable)
			{
				ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffer, m_RenderCommandEncoder);

				m_RenderCommandEncoder->popDebugGroup();
				m_RenderCommandEncoder->endEncoding();
				m_CommandBuffer->presentDrawable(m_Drawable);
				m_CommandBuffer->commit();

				m_RenderCommandEncoder = nullptr;
				m_CommandBuffer = nullptr;
				m_Drawable = nullptr;
			}
		}
#endif

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}
