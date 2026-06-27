#pragma once

#include "CoreTypes.h"
#include "Layer.h"

#include <vector>
#include <string>

namespace DotEngine {

    class DOTENGINE_API LayerStack {
    public:
        LayerStack()  = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);
        [[nodiscard]] Layer* GetLayer(const std::string& name) const;

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end()   { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int        m_LayerInsertIndex = 0;
    };

}
