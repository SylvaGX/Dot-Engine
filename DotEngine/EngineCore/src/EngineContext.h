#pragma once

#include "GraphicsContext.h"
#include "Events/EventQueue.h"
#include "LayerStack.h"

#include <string>
#include <cstdint>

namespace DotEngine {

    struct WindowData {
        std::string title;
        uint32_t    width         = 1280;
        uint32_t    height        = 720;
        bool        vsync         = true;
        void*       nativeHandle  = nullptr; // GLFWwindow*
    };

    struct EngineContext {
        GraphicsContext graphics;
        EventQueue      events;
        WindowData      window;
        LayerStack      layers;
        bool            running = true;
        // Reserved for ECS integration — see Sparse-Set-Driven ECS plan
        // void*        ecsWorld = nullptr;
    };

}
