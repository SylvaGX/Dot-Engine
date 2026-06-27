#include "OpenGLContext.h"

#include "GraphicsContext.h"
#include "Log.h"
#include "CoreTypes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DotEngine::Backend::OpenGL {

    void Init(GraphicsContext& ctx, void* glfwWindowHandle) {
        ctx.api                  = GraphicsAPI::OpenGL;
        ctx.opengl.windowHandle  = glfwWindowHandle;

        auto* window = static_cast<GLFWwindow*>(glfwWindowHandle);
        glfwMakeContextCurrent(window);

        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        DOTENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");

        DOTENGINE_CORE_INFO("OpenGL Info:");
        DOTENGINE_CORE_INFO("  Vendor:   {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        DOTENGINE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        DOTENGINE_CORE_INFO("  Version:  {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }

    void SwapBuffers(GraphicsContext& ctx) {
        auto* window = static_cast<GLFWwindow*>(ctx.opengl.windowHandle);
        glfwSwapBuffers(window);
    }

    void Shutdown(GraphicsContext& ctx) {
        ctx.opengl = {};
    }

}
