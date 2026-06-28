#include "DemoPanel.h"

#include "KeyCodes.h"
#include "Log.h"
#include "Events/EventTypes.h"

#include "imgui.h"

namespace Sandbox {

    void DemoPanel_Draw(DotEngine::EngineContext& /*ctx*/, DemoPanelState& state) {
        if (!state.dockingEnabled)
            return;

        bool open = true;

        ImGui::ShowDemoWindow(&open);
    }

}