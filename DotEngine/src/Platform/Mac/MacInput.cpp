#include "detpch.h"
#include "MacInput.h"

#include <GLFW/glfw3.h>
#include "DotEngine/Application.h"

namespace DotEngine {

	Input* Input::s_Instance = new MacInput();

	bool MacInput::IsKeyPressedImpl(const int keycode)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		const auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool MacInput::IsMouseButtonPressedImpl(const int button)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		const auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> MacInput::GetMousePosImpl()
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos {0}, ypos {0};

		glfwGetCursorPos(window, &xpos, &ypos);

		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	float MacInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePosImpl();

		return x;
	}
	float MacInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePosImpl();

		return y;
	}
}