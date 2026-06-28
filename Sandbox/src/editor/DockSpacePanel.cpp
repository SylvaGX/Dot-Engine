#include "DockSpacePanel.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Sandbox {

    void DockSpacePanel_Draw(DotEngine::EngineContext& /*ctx*/, SandboxEditorState& state) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                     |  ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;

        state.dockspace.dockspaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
        state.dockspace.dockspaceFlags |= ImGuiDockNodeFlags_NoWindowMenuButton
                                       |  ImGuiDockNodeFlags_NoCloseButton;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DotEngine DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        if (const ImGuiIO& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            const ImGuiID dockspace_id = ImGui::GetID("DotEngineDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), state.dockspace.dockspaceFlags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Test Window"))
                state.example.dockingEnabled = true;

            if (ImGui::BeginMenu("Options")) {
                if (ImGui::MenuItem("Flag: NoSplit", "",
                    (state.dockspace.dockspaceFlags & ImGuiDockNodeFlags_NoSplit) != 0))
                    state.dockspace.dockspaceFlags ^= ImGuiDockNodeFlags_NoSplit;
                if (ImGui::MenuItem("Flag: NoResize", "",
                    (state.dockspace.dockspaceFlags & ImGuiDockNodeFlags_NoResize) != 0))
                    state.dockspace.dockspaceFlags ^= ImGuiDockNodeFlags_NoResize;
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                    (state.dockspace.dockspaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                    state.dockspace.dockspaceFlags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "",
                    (state.dockspace.dockspaceFlags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                    state.dockspace.dockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
                    (state.dockspace.dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
                    state.dockspace.dockspaceFlags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                ImGui::Separator();
                if (ImGui::MenuItem("Close")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

}
