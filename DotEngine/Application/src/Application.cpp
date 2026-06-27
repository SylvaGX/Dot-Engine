#include "Application.h"

#include "Log.h"
#include "CoreTypes.h"
#include "PlatformInterface.h"
#include "RenderSystem.h"
#include "UISystem.h"
#include "Events/EventTypes.h"

namespace DotEngine {

    Application* Application::s_Instance = nullptr;

    Application::Application(const WindowProps& props)
        : m_Props(props)
    {
        DOTENGINE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
    }

    Application::~Application() {
        s_Instance = nullptr;
    }

    void Application::Run() {
        Platform::Init(m_Ctx, m_Props);
        Renderer::Init(m_Ctx);
        UI::Init(m_Ctx);

        while (m_Ctx.running) {
            Platform::PollEvents(m_Ctx);

            // Dispatch accumulated events to layers (back-to-front)
            for (auto& event : m_Ctx.events) {
                for (auto it = m_Ctx.layers.end(); it != m_Ctx.layers.begin();) {
                    (*--it)->OnEvent(event);
                    if (event.handled) break;
                }
            }
            m_Ctx.events.clear();

            Renderer::Clear(0.1f, 0.1f, 0.1f);

            for (Layer* layer : m_Ctx.layers)
                layer->OnUpdate();

            UI::Begin(m_Ctx);
            for (Layer* layer : m_Ctx.layers)
                layer->OnImGuiRender();
            UI::End(m_Ctx);

            Platform::Present(m_Ctx);
        }

        UI::Shutdown(m_Ctx);
        Platform::Shutdown(m_Ctx);
    }

    void Application::PushLayer(Layer* layer) {
        m_Ctx.layers.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay) {
        m_Ctx.layers.PushOverlay(overlay);
    }

    Layer* Application::GetLayer(const std::string& name) const {
        return m_Ctx.layers.GetLayer(name);
    }

}
