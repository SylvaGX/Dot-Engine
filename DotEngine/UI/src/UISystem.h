#pragma once

namespace DotEngine {
    struct EngineContext;
}

namespace DotEngine::UI {

    void Init(EngineContext& ctx);
    void Begin(EngineContext& ctx);
    void End(EngineContext& ctx);
    void Shutdown(EngineContext& ctx);

}
