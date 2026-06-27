#include "ShaderSystem.h"
#include "RenderTypes.h"

// Metal shader stubs — full implementation uses MTL::Library and MTL::Function.
// These satisfy the linker; expand when Metal rendering is fully implemented.

namespace DotEngine::Renderer {

    Shader CreateShader(const std::string& /*vertexSrc*/, const std::string& /*fragmentSrc*/) {
        return {};
    }

    void BindShader(const Shader& /*shader*/)  {}
    void DestroyShader(Shader& /*shader*/)     {}

}
