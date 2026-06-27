### Handling Global World State in C++ with Custom Initialization

This document demonstrates how to handle a global, capitalized `World` type that can either be initialized explicitly with custom dimensions or automatically fall back to defaults if accessed beforehand.

### Code Implementation

```cpp
#include <iostream>
#include <mutex>
#include <optional>
#include <utility>

// 1. Define the public data structure
class World {
public:
    std::pair<uint32_t, uint32_t> dimensions;

    // Explicit constructor
    World(uint32_t width, uint32_t height) : dimensions(width, height) {}

    /// Explicitly initialize the world with custom dimensions
    static void init(uint32_t width, uint32_t height) {
        std::call_once(init_flag, [width, height]() {
            instance.emplace(width, height);
        });
    }

    /// Access the global world width safely
    static uint32_t width() {
        ensure_initialized();
        return instance->dimensions.first;
    }

    /// Access the global world height safely
    static uint32_t height() {
        ensure_initialized();
        return instance->dimensions.second;
    }

private:
    // 2. Private static instance and synchronization flag
    // In C++, we use std::optional combined with std::call_once to emulate OnceLock behavior thread-safely
    static inline std::optional<World> instance;
    static inline std::once_flag init_flag;

    // 3. Define a standard helper function to generate fallback defaults
    static World create_default_world() {
        return World(800, 600);
    }

    static void ensure_initialized() {
        std::call_once(init_flag, []() {
            instance.emplace(create_default_world());
        });
    }
};

int main() {
    // Scenario: Explicitly initializing with custom dimensions
    World::init(1920, 1080);

    // This will now use the custom dimensions instead of the fallback defaults
    std::cout << "Width: " << World::width() << "\n";   // Prints 1920
    std::cout << "Height: " << World::height() << "\n"; // Prints 1080

    return 0;
}

```

### Key Mechanisms

* **`std::optional` and `std::once_flag**`: C++ doesn't have a direct equivalent to Rust's `OnceLock` in the standard library. By pairing a `std::optional` (which manages lifecycle and storage without raw pointer manipulation) with `std::once_flag`, we safely emulate its capabilities.
* **`std::call_once`**: Guarantees that the initialization closure runs exactly once, even if multiple execution threads attempt to access or initialize the global state simultaneously.
* **`World::init(width, height)`**: Allows you to explicitly set up the world size at startup. Because it routes through `std::call_once`, it maintains absolute thread-safety and prevents overwriting an already established state.
* **Fallback Protection**: If any code queries `World::width()` or `World::height()` before your custom `World::init()` executes, the system routes through the internal `ensure_initialized()` checkpoint to seamlessly instantiate the fallback defaults.