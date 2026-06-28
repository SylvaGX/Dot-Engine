#pragma once

namespace DotEngine {

    struct EngineContext;
    struct ApplicationContext;

    namespace Platform {
        void Init(ApplicationContext& app);
        void PollEvents(EngineContext& ctx);
        void Present(EngineContext& ctx);
        void Shutdown(EngineContext& ctx);
    }

}
