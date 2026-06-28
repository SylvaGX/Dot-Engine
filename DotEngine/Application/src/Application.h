#pragma once

#include "CoreMacros.h"
#include "ApplicationContext.h"
#include "WindowTypes.h"

namespace DotEngine {

    class DOTENGINE_API Application {
    public:
        explicit Application(WindowData window = WindowData());
        virtual ~Application();

        void Run();

    protected:
#ifdef DOT_EDITOR
        virtual void RegisterEditorPanels(EngineContext& ctx, EditorContext& editor) {}
#endif

        ApplicationContext m_App;
    };

    Application* createApplication();

}
