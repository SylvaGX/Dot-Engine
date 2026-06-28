#include "Platform.h"
#include "ApplicationContext.h"
#include "EngineContext.h"
#include "Events/EventSystem.h"
#include "Events/EventTypes.h"
#include "Log.h"
#include "CoreMacros.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace DotEngine {

    static void GLFWErrorCallback(int error, const char* description) {
        DOTENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    static ApplicationContext* GetApp(GLFWwindow* window) {
        return static_cast<ApplicationContext*>(glfwGetWindowUserPointer(window));
    }

    void Platform::Init(ApplicationContext& app) {
        EngineContext& ctx = app.engine;

        DOTENGINE_CORE_INFO("Creating window {0} ({1}, {2})", ctx.window.title, ctx.window.width, ctx.window.height);

        int success = glfwInit();
        DOTENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);

        GLFWwindow* glfwWindow = glfwCreateWindow(
            static_cast<int>(ctx.window.width),
            static_cast<int>(ctx.window.height),
            ctx.window.title.c_str(), nullptr, nullptr
        );

        if (!glfwWindow) {
            glfwTerminate();
            DOTENGINE_CORE_CRITICAL("Could not create GLFW window!");
            return;
        }

        if (ctx.window.maximized)
            glfwMaximizeWindow(glfwWindow);
        else
            glfwSetWindowPos(glfwWindow, ctx.window.x_position, ctx.window.y_position);

        ctx.window.nativeHandle = glfwWindow;
        glfwSetWindowUserPointer(glfwWindow, &app);

        Backend::OpenGL::Init(ctx.graphics, glfwWindow);
        glfwSwapInterval(ctx.window.vsync ? 1 : 0);

        glfwSetWindowPosCallback(glfwWindow, [](GLFWwindow* w, int x, int y) {
            auto* application = GetApp(w);
            if (!application)
                return;

            application->engine.window.x_position = x;
            application->engine.window.y_position = y;

            Event e;
            e.type        = EventType::WindowMoved;
            e.windowPos.x = x;
            e.windowPos.y = y;
            Events::Push(application->engine, e);
        });

        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* w, int width, int height) {
            auto* application = GetApp(w);
            if (!application)
                return;

            application->engine.window.width  = static_cast<uint32_t>(width);
            application->engine.window.height = static_cast<uint32_t>(height);

            Event e;
            e.type         = EventType::WindowResize;
            e.resize.width  = static_cast<uint32_t>(width);
            e.resize.height = static_cast<uint32_t>(height);
            Events::Push(application->engine, e);
        });

        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* w) {
            auto* application = GetApp(w);
            if (!application)
                return;

            application->engine.running = false;

            Event e;
            e.type = EventType::WindowClose;
            Events::Push(application->engine, e);
        });

        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* w, int key, int /*scancode*/, int action, int /*mods*/) {
            auto* application = GetApp(w);
            if (!application)
                return;

            Event e;
            switch (action) {
                case GLFW_PRESS:
                    e.type           = EventType::KeyPressed;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 0;
                    Events::Push(application->engine, e);
                    break;
                case GLFW_RELEASE:
                    e.type           = EventType::KeyReleased;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 0;
                    Events::Push(application->engine, e);
                    break;
                case GLFW_REPEAT:
                    e.type           = EventType::KeyPressed;
                    e.key.keyCode    = key;
                    e.key.repeatCount = 1;
                    Events::Push(application->engine, e);
                    break;
            }
        });

        glfwSetCharCallback(glfwWindow, [](GLFWwindow* w, unsigned int codepoint) {
            auto* application = GetApp(w);
            if (!application)
                return;

            Event e;
            e.type             = EventType::KeyTyped;
            e.key.keyCode      = static_cast<int>(codepoint);
            e.key.repeatCount  = 0;
            Events::Push(application->engine, e);
        });

        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* w, int button, int action, int /*mods*/) {
            auto* application = GetApp(w);
            if (!application)
                return;

            Event e;
            e.mouseButton.button = button;
            if (action == GLFW_PRESS)
                e.type = EventType::MouseButtonPressed;
            else if (action == GLFW_RELEASE)
                e.type = EventType::MouseButtonReleased;
            else
                return;
            Events::Push(application->engine, e);
        });

        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* w, double xOffset, double yOffset) {
            auto* application = GetApp(w);
            if (!application)
                return;

            Event e;
            e.type               = EventType::MouseScrolled;
            e.mouseScroll.xOffset = static_cast<float>(xOffset);
            e.mouseScroll.yOffset = static_cast<float>(yOffset);
            Events::Push(application->engine, e);
        });

        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* w, double xPos, double yPos) {
            auto* application = GetApp(w);
            if (!application)
                return;

            Event e;
            e.type          = EventType::MouseMoved;
            e.mouseMove.x   = static_cast<float>(xPos);
            e.mouseMove.y   = static_cast<float>(yPos);
            Events::Push(application->engine, e);
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
