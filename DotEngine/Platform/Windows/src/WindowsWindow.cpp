#include "PlatformInterface.h"
#include "EngineContext.h"
#include "Window.h"
#include "Events/EventTypes.h"
#include "Log.h"
#include "CoreTypes.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace DotEngine::Platform {
    void RegisterWindowForInput(GLFWwindow* window);
}

namespace DotEngine {

    static void GLFWErrorCallback(int error, const char* description) {
        DOTENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    void Platform::Init(EngineContext& ctx, const WindowProps& props) {
        ctx.window.title  = props.title;
        ctx.window.width  = props.width;
        ctx.window.height = props.height;

        DOTENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        int success = glfwInit();
        DOTENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);

        GLFWwindow* window = glfwCreateWindow(
            static_cast<int>(props.width),
            static_cast<int>(props.height),
            props.title.c_str(), nullptr, nullptr
        );
        DOTENGINE_CORE_ASSERT(window, "Failed to create GLFW window!");

        ctx.window.nativeHandle = window;
        glfwSetWindowUserPointer(window, &ctx);
        Platform::RegisterWindowForInput(window);

        // Initialize OpenGL backend
        Backend::OpenGL::Init(ctx.graphics, window);
        glfwSwapInterval(ctx.window.vsync ? 1 : 0);

        // --- GLFW event callbacks ---

        glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int width, int height) {
            auto& c       = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            c.window.width  = static_cast<uint32_t>(width);
            c.window.height = static_cast<uint32_t>(height);
            Event e;
            e.type         = EventType::WindowResize;
            e.resize.width  = static_cast<uint32_t>(width);
            e.resize.height = static_cast<uint32_t>(height);
            c.events.push_back(e);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* w) {
            auto& c   = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            c.running = false;
            Event e;
            e.type    = EventType::WindowClose;
            c.events.push_back(e);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int /*scancode*/, int action, int /*mods*/) {
            auto& c = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            Event e;
            switch (action) {
                case GLFW_PRESS:
                    e.type           = EventType::KeyPressed;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 0;
                    c.events.push_back(e);
                    break;
                case GLFW_RELEASE:
                    e.type           = EventType::KeyReleased;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 0;
                    c.events.push_back(e);
                    break;
                case GLFW_REPEAT:
                    e.type           = EventType::KeyPressed;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 1;
                    c.events.push_back(e);
                    break;
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* w, unsigned int codepoint) {
            auto& c            = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            Event e;
            e.type             = EventType::KeyTyped;
            e.key.keyCode      = static_cast<int>(codepoint);
            e.key.repeatCount  = 0;
            c.events.push_back(e);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int /*mods*/) {
            auto& c       = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            Event e;
            e.mouseButton.button = button;
            if (action == GLFW_PRESS)
                e.type = EventType::MouseButtonPressed;
            else if (action == GLFW_RELEASE)
                e.type = EventType::MouseButtonReleased;
            else
                return;
            c.events.push_back(e);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset) {
            auto& c              = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            Event e;
            e.type               = EventType::MouseScrolled;
            e.mouseScroll.xOffset = static_cast<float>(xOffset);
            e.mouseScroll.yOffset = static_cast<float>(yOffset);
            c.events.push_back(e);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xPos, double yPos) {
            auto& c         = *static_cast<EngineContext*>(glfwGetWindowUserPointer(w));
            Event e;
            e.type          = EventType::MouseMoved;
            e.mouseMove.x   = static_cast<float>(xPos);
            e.mouseMove.y   = static_cast<float>(yPos);
            c.events.push_back(e);
        });
    }

    void Platform::PollEvents(EngineContext& /*ctx*/) {
        glfwPollEvents();
    }

    void Platform::Present(EngineContext& ctx) {
        auto* window = static_cast<GLFWwindow*>(ctx.window.nativeHandle);
        glfwSwapBuffers(window);
    }

    void Platform::Shutdown(EngineContext& ctx) {
        auto* window = static_cast<GLFWwindow*>(ctx.window.nativeHandle);
        if (window) {
            Backend::OpenGL::Shutdown(ctx.graphics);
            glfwDestroyWindow(window);
        }
        glfwTerminate();
        ctx.window.nativeHandle = nullptr;
    }

}
