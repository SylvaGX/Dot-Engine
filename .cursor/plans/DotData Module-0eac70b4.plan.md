<!-- 0eac70b4-afe2-465c-af04-09a9417ff03d -->
---
todos:
  - id: "cmake-root"
    content: "Add find_package(nlohmann_json REQUIRED) and add_subdirectory(DotEngine/Data) to root CMakeLists.txt"
    status: pending
  - id: "data-cmake"
    content: "Create DotEngine/Data/CMakeLists.txt as a static lib linking nlohmann_json::nlohmann_json"
    status: pending
  - id: "versioning"
    content: "Create DataVersion.h with Version type and JSON/binary header structs"
    status: pending
  - id: "reflection"
    content: "Create Reflect.h with TypeInfo, FieldInfo, and DOTDATA_REFLECT_* macros"
    status: pending
  - id: "storage"
    content: "Create StorageLayer.h/cpp and StoragePaths.h for filesystem abstraction"
    status: pending
  - id: "serializer"
    content: "Create JsonSerializer.h/cpp implementing versioned Save/Load using nlohmann + reflection"
    status: pending
  - id: "validator"
    content: "Create Validator.h with rule-based ValidationResult system"
    status: pending
  - id: "umbrella"
    content: "Create DotData.h umbrella include and link DotData to DotApplication"
    status: pending
  - id: "demo"
    content: "Add WindowConfig reflection + save/load demo usage in SandBoxApp.cpp (optional, if user wants it)"
    status: pending
isProject: false
---
# DotData Module Implementation Plan

## Project Context

- C++23, CMake 3.16+, one static lib per module
- Existing modules: `DotEngineCore` → `DotRenderer` + `DotUI` → `DotApplication` → `Sandbox`
- Each module: `DotEngine/<Name>/CMakeLists.txt` + `DotEngine/<Name>/src/`
- JSON lib: **nlohmann/json** via Conan (same pattern as `spdlog` and `glm` — `find_package` + `nlohmann_json::nlohmann_json`)

---

## New Module Layout

```
DotEngine/Data/
├── CMakeLists.txt
└── src/
    ├── DotData.h                    ← umbrella include
    ├── Versioning/
    │   └── DataVersion.h            ← Version type + mandatory header structs
    ├── Reflection/
    │   ├── Reflect.h                ← TypeInfo, FieldInfo, macros
    │   └── Reflect.cpp              ← registry (if needed)
    ├── Serialization/
    │   ├── JsonSerializer.h         ← template Save/Load
    │   └── JsonSerializer.cpp
    ├── Validation/
    │   └── Validator.h              ← rule-based Validator<T>
    └── Storage/
        ├── StorageLayer.h           ← raw FS read/write
        ├── StorageLayer.cpp
        └── StoragePaths.h           ← %APPDATA%/DotEngine/, <proj>/.dotdata/
```

---

## Core API Design

### Versioning (`DataVersion.h`)
```cpp
namespace DotData {
    using Version = uint32_t;
    constexpr Version INVALID_VERSION = 0;
}
```

### Reflection (`Reflect.h`)
Manual reflection via macros — no external dependency, mirrors existing `RenderTypes.h` style:
```cpp
namespace DotData {
    enum class FieldType { Int, UInt32, Float, Double, Bool, String };

    struct FieldInfo { std::string_view name; FieldType type; size_t offset; };
    struct TypeInfo  { std::string_view typeName; std::vector<FieldInfo> fields; };

    template<typename T> const TypeInfo& GetTypeInfo(); // specialized per struct
}

// Usage in client code:
DOTDATA_REFLECT_BEGIN(WindowConfig)
    DOTDATA_FIELD(WindowConfig, width,      FieldType::UInt32)
    DOTDATA_FIELD(WindowConfig, height,     FieldType::UInt32)
    DOTDATA_FIELD(WindowConfig, fullscreen, FieldType::Bool)
DOTDATA_REFLECT_END()
```

### JSON Serializer (`JsonSerializer.h`)
```cpp
namespace DotData {
    class JsonSerializer {
    public:
        // Saves with mandatory {"version":N,"type":"T","data":{...}} envelope
        template<typename T>
        static bool Save(const T& obj, const std::filesystem::path& path, Version version);

        // Validates version field exists before deserializing
        template<typename T>
        static bool Load(T& obj, const std::filesystem::path& path, Version& outVersion);
    };
}
```

### Validator (`Validator.h`)
```cpp
namespace DotData {
    struct ValidationResult { bool valid = true; std::string error; };

    template<typename T>
    class Validator {
    public:
        using Rule = std::function<ValidationResult(const T&)>;
        void             AddRule(Rule rule);
        ValidationResult Validate(const T& obj) const;
    private:
        std::vector<Rule> m_Rules;
    };
}
```

### Storage Layer (`StorageLayer.h` / `StoragePaths.h`)
```cpp
namespace DotData {
    class StorageLayer {
    public:
        static bool WriteText(const std::filesystem::path& path, std::string_view content);
        static bool ReadText (const std::filesystem::path& path, std::string& outContent);
        static bool Exists   (const std::filesystem::path& path);
        static bool EnsureDir(const std::filesystem::path& path);
    };

    namespace Paths {
        std::filesystem::path EngineData();                                     // %APPDATA%/DotEngine/
        std::filesystem::path ProjectData(const std::filesystem::path& root);  // <root>/.dotdata/
    }
}
```

---

## Files to Create

- `DotEngine/Data/CMakeLists.txt`
- `DotEngine/Data/src/DotData.h`
- `DotEngine/Data/src/Versioning/DataVersion.h`
- `DotEngine/Data/src/Reflection/Reflect.h`
- `DotEngine/Data/src/Serialization/JsonSerializer.h`
- `DotEngine/Data/src/Serialization/JsonSerializer.cpp`
- `DotEngine/Data/src/Validation/Validator.h`
- `DotEngine/Data/src/Storage/StorageLayer.h`
- `DotEngine/Data/src/Storage/StorageLayer.cpp`
- `DotEngine/Data/src/Storage/StoragePaths.h`

---

## Files to Modify

- [CMakeLists.txt](CMakeLists.txt) — add `find_package(nlohmann_json REQUIRED)` and `add_subdirectory(DotEngine/Data)` before engine modules
- [DotEngine/Application/CMakeLists.txt](DotEngine/Application/CMakeLists.txt) — link `DotData` so `Application` can load/save `WindowData` configs

---

## Conan Dependency

nlohmann/json is already managed by Conan. In `Data/CMakeLists.txt`:
```cmake
find_package(nlohmann_json REQUIRED)
target_link_libraries(DotData PUBLIC nlohmann_json::nlohmann_json)
```
No vendor changes needed.

---

## What is NOT implemented (per spec)

- No migration/upgrade system
- No binary format (`.dat`, `.sav`) — headers spec'd but no implementation
- No asset system, no save-game system
- No editor integration
