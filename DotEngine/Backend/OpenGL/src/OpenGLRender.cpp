#include "RenderSystem.h"
#include "EngineContext.h"
#include "GraphicsTypes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DotEngine::Renderer {

    void Init(EngineContext& ctx) {
        // OpenGL state is initialized in Backend::OpenGL::Init (context creation)
        // Additional global GL state can be set here
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void DrawIndexed(uint32_t vao, const IndexBuffer& ib, const Shader& sh) {
        glUseProgram(sh.rendererID);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<int>(ib.count), GL_UNSIGNED_INT, nullptr);
    }

    void Present(EngineContext& ctx) {
        auto* window = static_cast<GLFWwindow*>(ctx.graphics.opengl.windowHandle);
        glfwSwapBuffers(window);
    }

    void Shutdown(EngineContext& /*ctx*/) {
        // No explicit cleanup needed for OpenGL state
    }

}
