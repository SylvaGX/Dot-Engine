#pragma once

#include "imgui.h"

namespace Sandbox {

    struct ExamplePanelState {
        bool dockingEnabled = true;
        bool tabHeld        = false;
    };

    struct DemoPanelState
    {
        bool dockingEnabled = true;
        bool tabHeld        = false;
    };

    struct DockSpacePanelState {
        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    };

    struct SandboxEditorState {
        ExamplePanelState   example;
        DockSpacePanelState dockspace;
        DemoPanelState      demo;
    };

}
