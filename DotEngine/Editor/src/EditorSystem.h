#pragma once

namespace DotEngine {
    struct EngineContext;
    struct EditorContext;
}

namespace DotEngine::Editor {

    void Init(EngineContext& ctx, EditorContext& editor);
    void Shutdown(EditorContext& editor);

    void Begin(EngineContext& ctx, EditorContext& editor);
    void End(EngineContext& ctx, EditorContext& editor);

    void UpdateAll(EngineContext& ctx, EditorContext& editor);
    void DrawPanels(EngineContext& ctx, EditorContext& editor);

}
