#pragma once

#include <utility>

namespace DotEngine::Input {

    // Implementations provided by the linked Platform module (Windows/Mac)
    bool IsKeyPressed(int keycode);
    bool IsMouseButtonPressed(int button);
    std::pair<float, float> GetMousePos();
    float GetMouseX();
    float GetMouseY();

}
