#pragma once

#include "Events/EventQueue.h"

namespace DotEngine {
    struct Event;
    struct GameInputState;
}

namespace DotEngine::GameInput {

    void ProcessEvent(const Event& event, GameInputState& input);
    void ProcessEvents(const EventQueue& events, GameInputState& input);

    void BeginFrame(GameInputState& input);

    bool IsKeyHeld(const GameInputState& input, int key);
    bool IsKeyDown(const GameInputState& input, int key);
    bool IsKeyUp(const GameInputState& input, int key);

}
