#include "Input.h"
#include "Log.h"

#include <GLFW/glfw3.h>

// Module-level window handle set by Platform::Init
static GLFWwindow* s_WindowHandle = nullptr;

namespace DotEngine::Platform {
    // Called from WindowsWindow.cpp after window creation to register the handle for Input
    void RegisterWindowForInput(GLFWwindow* window) {
        s_WindowHandle = window;
    }
}

namespace DotEngine::Input {

    bool IsKeyPressed(int keycode) {
        if (!s_WindowHandle) return false;
        int state = glfwGetKey(s_WindowHandle, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool IsMouseButtonPressed(int button) {
        if (!s_WindowHandle) return false;
        int state = glfwGetMouseButton(s_WindowHandle, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> GetMousePos() {
        if (!s_WindowHandle) return {0.0f, 0.0f};
        double x, y;
        glfwGetCursorPos(s_WindowHandle, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    float GetMouseX() { return GetMousePos().first; }
    float GetMouseY() { return GetMousePos().second; }

}
