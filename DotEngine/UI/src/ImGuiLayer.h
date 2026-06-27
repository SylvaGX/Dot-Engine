#pragma once

#include "Layer.h"

#ifdef DOTENGINE_PLATFORM_MACOS
namespace MTL { class CommandBuffer; class RenderCommandEncoder; }
namespace CA  { class MetalDrawable; }
#endif

namespace DotEngine {

    struct EngineContext;

    class DOTENGINE_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        void SetContext(EngineContext* ctx) { m_Ctx = ctx; }

        void Begin();
        void End();

    private:
        EngineContext* m_Ctx = nullptr;
        float m_Time = 0.0f;

#ifdef DOTENGINE_PLATFORM_MACOS
        MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
        MTL::CommandBuffer*        m_CommandBuffer        = nullptr;
        CA::MetalDrawable*         m_Drawable             = nullptr;
#endif
    };

}
