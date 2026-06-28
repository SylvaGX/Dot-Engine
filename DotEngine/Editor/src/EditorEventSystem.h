#pragma once

namespace DotEngine {
    struct EngineContext;
    struct EditorContext;
}

namespace DotEngine::Editor {

    void ProcessEvents(EngineContext& ctx, EditorContext& editor);

}
