#include <DotEngine.h>

#include "editor/SandboxEditorTypes.h"
#include "editor/ExamplePanel.h"
#include "editor/DockSpacePanel.h"
#include "editor/DemoPanel.h"

class SandBox : public DotEngine::Application {
public:
    SandBox() = default;
    ~SandBox() override = default;

protected:
    void RegisterEditorPanels(DotEngine::EngineContext& ctx, DotEngine::EditorContext& editor) override {

        DotEngine::Editor::RegisterPanel(editor, {
            .onPanels = +[](DotEngine::EngineContext& c, DotEngine::EditorContext& /*editor*/, void* data) {
                DockSpacePanel_Draw(c, *static_cast<Sandbox::SandboxEditorState*>(data));
            },
            .userData = &m_Editor
        });

        DotEngine::Editor::RegisterPanel(editor, {
            .onUpdate = +[](DotEngine::EngineContext& c, DotEngine::EditorContext& /*editor*/, void* data) {
                ExamplePanel_Update(c, *static_cast<Sandbox::ExamplePanelState*>(data));
            },
            .onPanels = +[](DotEngine::EngineContext& c, DotEngine::EditorContext& /*editor*/, void* data) {
                ExamplePanel_Draw(c, *static_cast<Sandbox::ExamplePanelState*>(data));
            },
            .onEvent = +[](DotEngine::EngineContext& c, DotEngine::EditorContext& /*editor*/, DotEngine::Event& e, void* data) {
                ExamplePanel_OnEvent(c, e, *static_cast<Sandbox::ExamplePanelState*>(data));
            },
            .userData = &m_Editor.example
        });

        DotEngine::Editor::RegisterPanel( editor, {
            .onPanels = +[](DotEngine::EngineContext& c, DotEngine::EditorContext& /*editor*/, void* data)
            {
                DemoPanel_Draw(c, *static_cast<Sandbox::DemoPanelState*>(data));
            },
            .userData = &m_Editor.demo
        });
    }

private:
    Sandbox::SandboxEditorState m_Editor{};
};

DotEngine::Application* DotEngine::createApplication() {
    return new SandBox();
}
