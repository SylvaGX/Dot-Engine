#pragma once

#include "DotEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace DotEngine {

	class OpenGLContext : public GraphicsContext {

	public:
		explicit OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
		[[nodiscard]] GraphicsAPI GetAPI() const override { return GraphicsAPI::OpenGL; }
		[[nodiscard]] void* GetDevice() const override { return nullptr; }
	private:
		GLFWwindow* m_WindowHandle;

	};

}