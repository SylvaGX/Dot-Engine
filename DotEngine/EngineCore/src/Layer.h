#pragma once

#include "CoreTypes.h"
#include "Events/EventTypes.h"

#include <string>

namespace DotEngine {

    class DOTENGINE_API Layer {
    public:
        explicit Layer(const std::string& debug_name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        [[nodiscard]] const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

}
