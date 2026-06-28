#pragma once

#include <vector>

namespace DotEngine {

    struct EngineContext;
    struct EditorContext;
    struct Event;

    struct EditorPanelEntry {
        void (*onUpdate)(EngineContext& ctx, EditorContext& editor, void* userData)                = nullptr;
        void (*onPanels)(EngineContext& ctx, EditorContext& editor, void* userData)                = nullptr;
        void (*onEvent)(EngineContext& ctx, EditorContext& editor, Event& event, void* userData) = nullptr;
        void* userData = nullptr;
    };

    struct EditorPanelRegistry {
        std::vector<EditorPanelEntry> entries;
    };

    struct EditorContext {
        bool attached       = false;
        bool frameActive    = false;
        void* renderEncoder = nullptr;
        void* commandBuffer = nullptr;
        void* drawable      = nullptr;
        EditorPanelRegistry panels;
    };

}
