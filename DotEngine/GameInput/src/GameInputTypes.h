#pragma once

namespace DotEngine {

    struct GameInputState {
        static constexpr int KeyCount = 512;
        static constexpr int MouseButtonCount = 8;

        bool  keysHeld[KeyCount]{};
        bool  keysDown[KeyCount]{};
        bool  keysUp[KeyCount]{};
        bool  mouseButtonsHeld[MouseButtonCount]{};
        float mouseX    = 0.f;
        float mouseY    = 0.f;
        float scrollX   = 0.f;
        float scrollY   = 0.f;
    };

}
