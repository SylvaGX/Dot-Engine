#include "ExamplePanel.h"

#include "KeyCodes.h"
#include "Log.h"
#include "Events/EventTypes.h"

#include "imgui.h"

namespace Sandbox {

    void ExamplePanel_Update(DotEngine::EngineContext& /*ctx*/, ExamplePanelState& state) {
        if (!state.dockingEnabled)
            return;

        if (state.tabHeld)
            DOTENGINE_TRACE("Tab key is held (event)!");
    }

    void ExamplePanel_Draw(DotEngine::EngineContext& /*ctx*/, ExamplePanelState& state) {
        if (!state.dockingEnabled)
            return;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        if (ImGui::Begin("Test", &state.dockingEnabled, window_flags))
            ImGui::Text("Hello World");
        ImGui::End();
    }

    void ExamplePanel_OnEvent(DotEngine::EngineContext& /*ctx*/, DotEngine::Event& event, ExamplePanelState& state) {
        if (!state.dockingEnabled)
            return;

        if (event.type == DotEngine::EventType::KeyPressed && event.key.keyCode == DOTENGINE_KEY_TAB)
            state.tabHeld = true;

        if (event.type == DotEngine::EventType::KeyReleased && event.key.keyCode == DOTENGINE_KEY_TAB)
            state.tabHeld = false;

        if (event.type == DotEngine::EventType::KeyPressed)
            DOTENGINE_TRACE("{0}", static_cast<char>(event.key.keyCode));
    }

}
