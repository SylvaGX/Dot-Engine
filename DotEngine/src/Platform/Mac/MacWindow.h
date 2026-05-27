#pragma once

#include "DotEngine/Window.h"
#include "DotEngine/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace DotEngine {

	class MacWindow : public Window{
	public:
		MacWindow(const WindowProps& props);
		~MacWindow() override;

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		// Window attribute
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void* GetNativeWindow() const override { return m_Window; }
		GraphicsContext* GetGraphicsContext() const override { return m_Context; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window{};
		GraphicsContext* m_Context{};

		struct WindowData {
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}