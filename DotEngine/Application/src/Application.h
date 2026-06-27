#pragma once

#include "CoreTypes.h"
#include "EngineContext.h"
#include "Window.h"

#include <string>

namespace DotEngine {

    class DOTENGINE_API Application {
    public:
        explicit Application(const WindowProps& props = WindowProps());
        virtual ~Application();

        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        [[nodiscard]] Layer* GetLayer(const std::string& name) const;

        [[nodiscard]] EngineContext&       GetContext()       { return m_Ctx; }
        [[nodiscard]] const EngineContext& GetContext() const { return m_Ctx; }

        static Application& Get() { return *s_Instance; }

    private:
        EngineContext m_Ctx;
        WindowProps   m_Props;

        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* createApplication();

}
