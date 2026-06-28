#include "GameInputSystem.h"

#include "Events/EventQueue.h"
#include "Events/EventTypes.h"
#include "GameInputTypes.h"

#include <GLFW/glfw3.h>

namespace DotEngine::GameInput {

    namespace {

        void OnKey(GameInputState& input, int key, int action) {
            if (key < 0 || key >= GameInputState::KeyCount)
                return;

            switch (action) {
                case GLFW_PRESS:
                    if (!input.keysHeld[key])
                        input.keysDown[key] = true;
                    input.keysHeld[key] = true;
                    break;
                case GLFW_RELEASE:
                    if (input.keysHeld[key])
                        input.keysUp[key] = true;
                    input.keysHeld[key] = false;
                    break;
                case GLFW_REPEAT:
                    input.keysDown[key] = true;
                    input.keysHeld[key] = true;
                    break;
            }
        }

        void OnMouseButton(GameInputState& input, int button, int action) {
            if (button < 0 || button >= GameInputState::MouseButtonCount)
                return;

            if (action == GLFW_PRESS)
                input.mouseButtonsHeld[button] = true;
            else if (action == GLFW_RELEASE)
                input.mouseButtonsHeld[button] = false;
        }

    }

    void ProcessEvent(const Event& event, GameInputState& input) {
        switch (event.type) {
            case EventType::KeyPressed:
                OnKey(input, event.key.keyCode, event.key.repeatCount > 0 ? GLFW_REPEAT : GLFW_PRESS);
                break;
            case EventType::KeyReleased:
                OnKey(input, event.key.keyCode, GLFW_RELEASE);
                break;
            case EventType::MouseButtonPressed:
                OnMouseButton(input, event.mouseButton.button, GLFW_PRESS);
                break;
            case EventType::MouseButtonReleased:
                OnMouseButton(input, event.mouseButton.button, GLFW_RELEASE);
                break;
            case EventType::MouseMoved:
                input.mouseX = event.mouseMove.x;
                input.mouseY = event.mouseMove.y;
                break;
            case EventType::MouseScrolled:
                input.scrollX = event.mouseScroll.xOffset;
                input.scrollY = event.mouseScroll.yOffset;
                break;
            default:
                break;
        }
    }

    void ProcessEvents(const EventQueue& events, GameInputState& input) {
        for (const auto& event : events)
            ProcessEvent(event, input);
    }

    void BeginFrame(GameInputState& input) {
        for (int key = 0; key < GameInputState::KeyCount; ++key) {
            input.keysDown[key] = false;
            input.keysUp[key]   = false;
        }
        input.scrollX = 0.f;
        input.scrollY = 0.f;
    }

    bool IsKeyHeld(const GameInputState& input, int key) {
        if (key < 0 || key >= GameInputState::KeyCount)
            return false;
        return input.keysHeld[key];
    }

    bool IsKeyDown(const GameInputState& input, int key) {
        if (key < 0 || key >= GameInputState::KeyCount)
            return false;
        return input.keysDown[key];
    }

    bool IsKeyUp(const GameInputState& input, int key) {
        if (key < 0 || key >= GameInputState::KeyCount)
            return false;
        return input.keysUp[key];
    }

}
