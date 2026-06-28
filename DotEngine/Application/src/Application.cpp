#include "Application.h"

#include "DotData.h"
#include "Log.h"
#include "CoreMacros.h"
#include "Platform.h"
#include "RenderSystem.h"
#include "Events/EventSystem.h"
#include "GameInputSystem.h"

#ifdef DOT_EDITOR
    #include "EditorSystem.h"
    #include "EditorPanelSystem.h"
    #include "EditorEventSystem.h"
#endif

namespace DotEngine {

    Application::Application(WindowData window)
    {
        DotData::Version v{};
        std::filesystem::path path = DotData::Paths::EngineData() / "WindowConfig.json";

        if (!DotData::JsonSerializer::Load<WindowData>(window, path, v))
            DOTENGINE_CORE_ERROR("Error loading window configuration");

        m_App.engine.window = window;
    }

    Application::~Application() = default;

    void Application::Run() {
        Platform::Init(m_App);
        Renderer::Init(m_App.engine);

#ifdef DOT_EDITOR
        Editor::Init(m_App.engine, m_App.editor);
        RegisterEditorPanels(m_App.engine, m_App.editor);
#endif

        while (m_App.engine.running) {
            Platform::PollEvents(m_App.engine);

            GameInput::BeginFrame(m_App.gameInput);
            GameInput::ProcessEvents(m_App.engine.events, m_App.gameInput);

#ifdef DOT_EDITOR
            Editor::ProcessEvents(m_App.engine, m_App.editor);
#endif

            Events::Clear(m_App.engine);

#ifdef DOT_EDITOR
            Editor::UpdateAll(m_App.engine, m_App.editor);
#endif

            Renderer::Clear(0.1f, 0.1f, 0.1f);

#ifdef DOT_EDITOR
            Editor::Begin(m_App.engine, m_App.editor);
            Editor::DrawPanels(m_App.engine, m_App.editor);
            Editor::End(m_App.engine, m_App.editor);
#endif

            Platform::Present(m_App.engine);
        }

#ifdef DOT_EDITOR
        Editor::Shutdown(m_App.editor);
#endif
        Platform::Shutdown(m_App.engine);
    }

}
