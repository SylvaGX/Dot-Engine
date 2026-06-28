#include "EditorEventSystem.h"

#include "EditorContext.h"
#include "EngineContext.h"
#include "Events/EventTypes.h"

namespace DotEngine::Editor {

    void ProcessEvents(EngineContext& ctx, EditorContext& editor) {
        for (auto& event : ctx.events) {
            for (auto it = editor.panels.entries.rbegin(); it != editor.panels.entries.rend(); ++it) {
                if (it->onEvent)
                    it->onEvent(ctx, editor, event, it->userData);
                if (event.handled)
                    break;
            }
        }
    }

}
