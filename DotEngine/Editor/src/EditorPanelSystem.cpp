#include "EditorPanelSystem.h"

#include "EditorContext.h"

namespace DotEngine::Editor {

    void RegisterPanel(EditorContext& editor, EditorPanelEntry entry) {
        editor.panels.entries.push_back(entry);
    }

}
