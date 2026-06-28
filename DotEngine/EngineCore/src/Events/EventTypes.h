#pragma once

#include <cstdint>

namespace DotEngine {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased,
        MouseMoved, MouseScrolled
    };

    struct Event {
        EventType type    = EventType::None;
        bool      handled = false;

        union {
            struct { int      keyCode; int repeatCount;  } key;
            struct { int      x, y;                      } windowPos;
            struct { float    x, y;                      } mouseMove;
            struct { float    xOffset, yOffset;          } mouseScroll;
            struct { int      button;                    } mouseButton;
            struct { uint32_t width, height;             } resize;
        };

        Event() : key{0, 0} {}
    };

}
