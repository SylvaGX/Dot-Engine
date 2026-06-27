#pragma once

#include "Versioning/DataVersion.h"
#include "Reflection/Reflect.h"
#include "Storage/StorageLayer.h"
#include "Validation/SchemaFromReflection.h"
#include "Validation/JsonSchemaValidator.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>

namespace DotData {

    // Reads/writes the field value from/into a reflected C++ struct via void* pointer
    // and the FieldInfo descriptor.
    namespace Detail {

        inline void FieldToJson(nlohmann::json& out, const void* base, const FieldInfo& f) {
            const void* ptr = static_cast<const char*>(base) + f.offset;
            switch (f.type) {
                case FieldType::Int:    out[f.name] = *static_cast<const int*>(ptr);          break;
                case FieldType::UInt32: out[f.name] = *static_cast<const uint32_t*>(ptr);     break;
                case FieldType::Float:  out[f.name] = *static_cast<const float*>(ptr);        break;
                case FieldType::Double: out[f.name] = *static_cast<const double*>(ptr);       break;
                case FieldType::Bool:   out[f.name] = *static_cast<const bool*>(ptr);         break;
                case FieldType::String: out[f.name] = *static_cast<const std::string*>(ptr);  break;
            }
        }

        inline bool FieldFromJson(const nlohmann::json& in, void* base, const FieldInfo& f) {
            if (!in.contains(f.name)) return false;
            void* ptr = static_cast<char*>(base) + f.offset;
            try {
                switch (f.type) {
                    case FieldType::Int:    *static_cast<int*>(ptr)         = in.at(f.name).get<int>();          break;
                    case FieldType::UInt32: *static_cast<uint32_t*>(ptr)    = in.at(f.name).get<uint32_t>();     break;
                    case FieldType::Float:  *static_cast<float*>(ptr)       = in.at(f.name).get<float>();        break;
                    case FieldType::Double: *static_cast<double*>(ptr)      = in.at(f.name).get<double>();       break;
                    case FieldType::Bool:   *static_cast<bool*>(ptr)        = in.at(f.name).get<bool>();         break;
                    case FieldType::String: *static_cast<std::string*>(ptr) = in.at(f.name).get<std::string>(); break;
                }
            } catch (...) {
                return false;
            }
            return true;
        }

    } // namespace Detail

    // Reflection-driven JSON serializer.
    //
    // Every file written by Save() follows the mandatory DotData envelope:
    //   {
    //     "version": <N>,
    //     "type":    "<TypeName>",
    //     "data":    { ... reflected fields ... }
    //   }
    //
    // Load() rejects files that are missing the "version" field entirely.
    class JsonSerializer {
    public:
        // Serializes obj to path using reflection metadata for T.
        // version must be > INVALID_VERSION (0).
        template<typename T>
        static bool Save(const T& obj,
                         const std::filesystem::path& path,
                         Version version)
        {
            if (version == INVALID_VERSION) return false;

            const TypeInfo& info = GetTypeInfo<T>();

            nlohmann::json data;
            for (const FieldInfo& field : info.fields)
                Detail::FieldToJson(data, &obj, field);

            nlohmann::json root;
            root["version"] = version;
            root["type"]    = std::string(info.typeName);
            root["data"]    = std::move(data);

            return StorageLayer::WriteText(path, root.dump(4));
        }

        // Deserializes obj from path.
        // outVersion receives the stored version number.
        // Returns false if the file is missing, unreadable, has no "version" field,
        // or if any mandatory field cannot be parsed.
        template<typename T>
        static bool Load(T& obj,
                         const std::filesystem::path& path,
                         Version& outVersion)
        {
            std::string raw;
            if (!StorageLayer::ReadText(path, raw)) return false;

            nlohmann::json root;
            try {
                root = nlohmann::json::parse(raw);
            } catch (...) {
                return false;
            }

            {
                const JsonSchemaValidator schemaValidator(BuildJsonSchema<T>());
                if (auto [valid, error] = schemaValidator.Validate(root); !valid) return false;
            }

            outVersion = root["version"].get<Version>();
            if (outVersion == INVALID_VERSION) return false;

            const TypeInfo&      info = GetTypeInfo<T>();
            const nlohmann::json& data = root["data"];

            for (const FieldInfo& field : info.fields) {
                if (!Detail::FieldFromJson(data, &obj, field))
                    return false;
            }

            return true;
        }
    };

} // namespace DotData
