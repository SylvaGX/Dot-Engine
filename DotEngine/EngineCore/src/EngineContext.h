#pragma once

#include "GraphicsTypes.h"
#include "WindowTypes.h"
#include "Events/EventQueue.h"

namespace DotEngine {

    struct EngineContext {
        GraphicsContext graphics;
        WindowData      window;
        EventQueue      events;
        bool            running = true;
    };

}
