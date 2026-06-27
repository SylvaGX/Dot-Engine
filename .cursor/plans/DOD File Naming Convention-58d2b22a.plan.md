<!-- 58d2b22a-aed5-4985-b881-400f531f6896 -->
---
todos:
  - id: "rename-files"
    content: "Rename the four misnamed header files: CoreTypes.h → CoreMacros.h, Window.h → WindowTypes.h, GraphicsContext.h → GraphicsTypes.h, PlatformInterface.h → Platform.h"
    status: pending
  - id: "merge-window-structs"
    content: "Merge WindowProps into WindowData: delete WindowProps, add fullscreen+vsync to WindowData, update reflection block to exclude nativeHandle, update all callers to use WindowData"
    status: pending
  - id: "update-includes"
    content: "Update all #include directives across the 13 affected files to use the new header names"
    status: pending
  - id: "update-mdc"
    content: "Append the file naming convention section to .cursor/rules/project.mdc"
    status: pending
isProject: false
---
# DOD File Naming Convention Refactor

## Convention Being Enforced

Five mutually exclusive file categories — a file must fit exactly one:

| Category | File suffix | Namespace rule | Content rule |
|---|---|---|---|
| Types | `*Types.h` | `DotEngine` (module root) | Structs, enums, POD handles only |
| System | `*System.h` / `.cpp` | `DotEngine::<FileStem>` | Free functions only |
| Context | `*Context.h` | `DotEngine` | One aggregate state struct, name matches file |
| Macros | `*Macros.h` | none | Preprocessor defines only |
| Class | `<ClassName>.h` / `.cpp` | `DotEngine` | One class, name matches file |

---

## Part 1 — File Renames

Four files are misnamed relative to their content or namespace:

- [`DotEngine/EngineCore/src/CoreTypes.h`](DotEngine/EngineCore/src/CoreTypes.h) → **`CoreMacros.h`**
  - Contains zero types; only preprocessor macros (`DOTENGINE_API`, `DOTENGINE_ASSERT`, `BIT`, etc.)

- [`DotEngine/EngineCore/src/Window.h`](DotEngine/EngineCore/src/Window.h) → **`WindowTypes.h`**
  - Contains `WindowProps` (no `Window` type exists). `WindowData` also gets moved here from `EngineContext.h`.

- [`DotEngine/EngineCore/src/GraphicsContext.h`](DotEngine/EngineCore/src/GraphicsContext.h) → **`GraphicsTypes.h`**
  - Contains `GraphicsAPI` (enum), `OpenGLContextData`, `MetalContextData`, and `GraphicsContext` — four distinct things, none of which warrant calling the file a "Context".

- [`DotEngine/EngineCore/src/PlatformInterface.h`](DotEngine/EngineCore/src/PlatformInterface.h) → **`Platform.h`**
  - The namespace inside is already `DotEngine::Platform`; the file name just needs to match.

---

## Part 2 — Merge WindowProps into WindowData

`WindowProps` (creation config) and `WindowData` (runtime state) are redundant. The DotData reflection system is field-opt-in, so `nativeHandle` can simply be omitted from the reflection block. Both structs are merged into one `WindowData` that lives in `WindowTypes.h`.

**Merged struct:**

```cpp
// WindowTypes.h
namespace DotEngine {
    struct WindowData {
        std::string title        = "Dot Engine";
        uint32_t    width        = 1280;
        uint32_t    height       = 720;
        bool        fullscreen   = false;
        bool        vsync        = true;
        void*       nativeHandle = nullptr;  // GLFWwindow* — not serialized
    };
}

DOTDATA_REFLECT_BEGIN(DotEngine::WindowData)
    DOTDATA_FIELD(DotEngine::WindowData, title,      DotData::FieldType::String)
    DOTDATA_FIELD(DotEngine::WindowData, width,      DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowData, height,     DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowData, fullscreen, DotData::FieldType::Bool)
    DOTDATA_FIELD(DotEngine::WindowData, vsync,      DotData::FieldType::Bool)
DOTDATA_REFLECT_END()
```

**Callers updated** (all uses of `WindowProps` replaced with `WindowData`):

- [`DotEngine/EngineCore/src/EngineContext.h`](DotEngine/EngineCore/src/EngineContext.h) — `WindowData` member already present; remove old `WindowProps` include
- [`DotEngine/Application/src/Application.h`](DotEngine/Application/src/Application.h) — constructor parameter `WindowProps` → `WindowData`; remove `m_Props` member (redundant, data lives in `m_Ctx.window`)
- [`DotEngine/Application/src/Application.cpp`](DotEngine/Application/src/Application.cpp) — update constructor signature
- [`DotEngine/Platform/Windows/src/WindowsWindow.cpp`](DotEngine/Platform/Windows/src/WindowsWindow.cpp) — `Platform::Init(EngineContext&, const WindowProps&)` → `const WindowData&`; remove the three-field copy since `ctx.window` is already the same struct
- [`DotEngine/Platform/Mac/src/MacWindow.cpp`](DotEngine/Platform/Mac/src/MacWindow.cpp) — same as above
- [`DotEngine/EngineCore/src/PlatformInterface.h`](DotEngine/EngineCore/src/PlatformInterface.h) (becomes `Platform.h`) — forward declaration `struct WindowProps` → `struct WindowData`

`EngineContext.h` then includes `WindowTypes.h` (which now carries `WindowData`) instead of the old `GraphicsContext.h`.

---

## Part 3 — Include Updates

Every file that `#include`s a renamed header must be updated. The full list (discovered via grep):

**`CoreTypes.h` → `CoreMacros.h`** (13 files):
- `DotEngine/EngineCore/src/Layer.h`
- `DotEngine/EngineCore/src/LayerStack.h`
- `DotEngine/EngineCore/src/Log.h`
- `DotEngine/Application/src/DotEngine.h`
- `DotEngine/Application/src/Application.h`
- `DotEngine/Application/src/Application.cpp`
- `DotEngine/Backend/OpenGL/src/OpenGLBuffer.cpp`
- `DotEngine/Backend/OpenGL/src/OpenGLContext.cpp`
- `DotEngine/Backend/OpenGL/src/OpenGLShader.cpp`
- `DotEngine/Backend/Metal/src/MetalContext.cpp`
- `DotEngine/UI/src/ImGuiLayer.cpp`
- `DotEngine/Platform/Windows/src/WindowsWindow.cpp`
- `DotEngine/Platform/Mac/src/MacWindow.cpp`

**`Window.h` → `WindowTypes.h`** (3 files):
- `DotEngine/Application/src/Application.h`
- `DotEngine/Platform/Windows/src/WindowsWindow.cpp`
- `DotEngine/Platform/Mac/src/MacWindow.cpp`

**`GraphicsContext.h` → `GraphicsTypes.h`** (5 files):
- `DotEngine/EngineCore/src/EngineContext.h`
- `DotEngine/Backend/OpenGL/src/OpenGLContext.cpp`
- `DotEngine/Backend/OpenGL/src/OpenGLRender.cpp`
- `DotEngine/Backend/Metal/src/MetalContext.cpp`
- `DotEngine/UI/src/ImGuiLayer.cpp`

**`PlatformInterface.h` → `Platform.h`** (3 files):
- `DotEngine/Application/src/Application.cpp`
- `DotEngine/Platform/Windows/src/WindowsWindow.cpp`
- `DotEngine/Platform/Mac/src/MacWindow.cpp`

---

## Part 4 — Update `project.mdc`

Append a new section to [`.cursor/rules/project.mdc`](.cursor/rules/project.mdc) documenting the five file categories so AI and future contributors follow the same rules:

```markdown
## File naming convention (DOD C++)

Every header must fit exactly one of these five categories:

- `*Types.h` — data only: structs, enums, POD handles. No methods. Namespace = module root (`DotEngine`, `DotData`).
- `*System.h` / `*System.cpp` — free functions that operate on types. Namespace must be `DotEngine::<FileStem>` (e.g. `BufferSystem.h` → `namespace DotEngine::Renderer`).
- `*Context.h` — one aggregate state struct. File name must equal type name. No other types.
- `*Macros.h` — preprocessor defines only. No types, no namespaces.
- `<ClassName>.h` / `<ClassName>.cpp` — one class per file, file name equals class name.

Rules:
- A `*Types.h` file may group multiple related POD structs (e.g. `RenderTypes.h`, `WindowTypes.h`).
- A `*System.h` file must not define types; forward-declare them from the matching `*Types.h`.
- Never mix categories in a single file.
```
