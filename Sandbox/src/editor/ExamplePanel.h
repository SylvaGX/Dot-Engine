#pragma once
#include "SandboxEditorTypes.h"

namespace DotEngine {
    struct EngineContext;
    struct Event;
}

namespace Sandbox {

    void ExamplePanel_Update(DotEngine::EngineContext& ctx, ExamplePanelState& state);
    void ExamplePanel_Draw(DotEngine::EngineContext& ctx, ExamplePanelState& state);
    void ExamplePanel_OnEvent(DotEngine::EngineContext& ctx, DotEngine::Event& event, ExamplePanelState& state);

}
