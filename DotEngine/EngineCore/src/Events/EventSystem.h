#pragma once

namespace DotEngine {
    struct EngineContext;
    struct Event;
}

namespace DotEngine::Events {

    void Push(EngineContext& ctx, Event event);
    void Clear(EngineContext& ctx);

}
