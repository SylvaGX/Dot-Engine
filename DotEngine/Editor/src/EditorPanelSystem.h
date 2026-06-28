#pragma once

namespace DotEngine {
    struct EditorContext;
    struct EditorPanelEntry;
}

namespace DotEngine::Editor {

    void RegisterPanel(EditorContext& editor, EditorPanelEntry entry);

}
