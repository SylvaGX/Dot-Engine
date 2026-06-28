#include "ShaderSystem.h"
#include "RenderTypes.h"
#include "Log.h"
#include "CoreMacros.h"

#include <glad/glad.h>
#include <vector>

namespace DotEngine::Renderer {

    Shader CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
        Shader shader;

        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* src   = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &src, 0);
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(vertexShader);
            DOTENGINE_CORE_ERROR("{0}", infoLog.data());
            DOTENGINE_CORE_ASSERT(false, "Vertex shader compilation failed!");
            return shader;
        }

        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        src = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &src, 0);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);
            DOTENGINE_CORE_ERROR("{0}", infoLog.data());
            DOTENGINE_CORE_ASSERT(false, "Fragment shader compilation failed!");
            return shader;
        }

        shader.rendererID  = glCreateProgram();
        const GLuint program     = shader.rendererID;
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            DOTENGINE_CORE_ERROR("{0}", infoLog.data());
            DOTENGINE_CORE_ASSERT(false, "Shader link failed!");
            shader.rendererID = 0;
            return shader;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shader;
    }

    void BindShader(const Shader& shader) {
        glUseProgram(shader.rendererID);
    }

    void DestroyShader(Shader& shader) {
        glDeleteProgram(shader.rendererID);
        shader.rendererID = 0;
    }

}
