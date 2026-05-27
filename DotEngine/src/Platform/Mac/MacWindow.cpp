#include "detpch.h"
#include "MacWindow.h"

#include "DotEngine/Events/ApplicationEvent.h"
#include "DotEngine/Events/MouseEvent.h"
#include "DotEngine/Events/KeyEvent.h"

#include "Platform/Metal/MetalContext.h"

namespace DotEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		DOTENGINE_CORE_ERROR("GLFW Erro ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new MacWindow(props);
	}

	MacWindow::MacWindow(const WindowProps& props) {
		MacWindow::Init(props);
	}

	MacWindow::~MacWindow() {
		MacWindow::Shutdown();
	}

	void MacWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DOTENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			DOTENGINE_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);

		if (!m_Window)
		{
			glfwTerminate();
			DOTENGINE_CORE_CRITICAL("Could not create window");
		}
		
		m_Context = new MetalContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const int key, int scancode, int action, int mods) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action) {
				case GLFW_PRESS:{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int c) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			KeyTypedEvent event(c);
			data.EventCallback(event);

		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, const int button, const int action, int mods) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow * window, const double xPos, const double yPos) {
			const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(event);
		});
	}

	void MacWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void MacWindow::OnUpdate() {
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void MacWindow::SetVSync(bool enabled) {
		/*
		 * glfwSwapInterval is specific to OpenGL & OpenGL ES.
		 * See the corresponding for Metal
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
			*/

		m_Data.VSync = enabled;
	}

	bool MacWindow::IsVSync() const {
		return m_Data.VSync;
	}

}