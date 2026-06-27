#pragma once

namespace DotEngine {

    struct EngineContext;
    struct WindowProps;

    // Implementations provided by the linked Platform module (Windows/Mac)
    namespace Platform {
        void Init(EngineContext& ctx, const WindowProps& props);
        void PollEvents(EngineContext& ctx);
        void Present(EngineContext& ctx);
        void Shutdown(EngineContext& ctx);
    }

}
