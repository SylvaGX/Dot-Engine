#include "UISystem.h"
#include "ImGuiLayer.h"
#include "EngineContext.h"

namespace DotEngine::UI {

    static ImGuiLayer* s_Layer = nullptr;

    void Init(EngineContext& ctx) {
        s_Layer = new ImGuiLayer();
        s_Layer->SetContext(&ctx);
        ctx.layers.PushOverlay(s_Layer);
    }

    void Begin(EngineContext& /*ctx*/) {
        if (s_Layer) s_Layer->Begin();
    }

    void End(EngineContext& /*ctx*/) {
        if (s_Layer) s_Layer->End();
    }

    void Shutdown(EngineContext& ctx) {
        if (s_Layer) {
            ctx.layers.PopOverlay(s_Layer);
            s_Layer = nullptr;
        }
    }

}
