#pragma once

#include "RenderTypes.h"

#include <string>

namespace DotEngine::Renderer {

    // Implementations provided by the linked Backend module (OpenGL/Metal)
    Shader CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    void   BindShader(const Shader& shader);
    void   DestroyShader(Shader& shader);

}
