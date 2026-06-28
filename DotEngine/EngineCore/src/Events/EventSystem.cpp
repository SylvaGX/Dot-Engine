#include "EventSystem.h"

#include "EngineContext.h"
#include "Events/EventTypes.h"

namespace DotEngine::Events {

    void Push(EngineContext& ctx, Event event) {
        ctx.events.push_back(event);
    }

    void Clear(EngineContext& ctx) {
        ctx.events.clear();
    }

}
