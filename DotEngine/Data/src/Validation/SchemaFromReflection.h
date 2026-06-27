#pragma once

#include "Reflection/Reflect.h"
#include "Versioning/DataVersion.h"
#include "Validation/BinarySchemaValidator.h"

#include <nlohmann/json.hpp>

#include <string_view>

namespace DotData {

    namespace Detail {

        inline const char* JsonTypeName(const FieldType type) {
            switch (type) {
                case FieldType::Int:
                case FieldType::UInt32: return "integer";
                case FieldType::Float:
                case FieldType::Double: return "number";
                case FieldType::Bool:   return "boolean";
                case FieldType::String: return "string";
            }
            return "string";
        }

        // Returns the fixed byte size of a reflected field type for binary payload size estimation.
        inline size_t FieldByteSize(const FieldType type) {
            switch (type) {
                case FieldType::Int:    return sizeof(int);
                case FieldType::UInt32: return sizeof(uint32_t);
                case FieldType::Float:  return sizeof(float);
                case FieldType::Double: return sizeof(double);
                case FieldType::Bool:   return sizeof(bool);
                case FieldType::String: return 0; // variable-length; caller treats 0 as "unknown"
            }
            return 0;
        }

    } // namespace Detail

    // Builds a JSON Schema object from the reflected TypeInfo of T.
    //
    // The schema enforces the DotData JSON envelope:
    //   { "version": <integer>, "type": <string>, "data": { ...fields... } }
    //
    // All reflected fields are placed under "data" as required properties.
    template<typename T>
    nlohmann::json BuildJsonSchema() {
        const TypeInfo& info = GetTypeInfo<T>();

        nlohmann::json dataProps = nlohmann::json::object();
        nlohmann::json dataRequired = nlohmann::json::array();

        for (const FieldInfo& field : info.fields) {
            dataProps[std::string(field.name)] = {{"type", Detail::JsonTypeName(field.type)}};
            dataRequired.push_back(std::string(field.name));
        }

        nlohmann::json schema;
        schema["type"]     = "object";
        schema["required"] = {"version", "type", "data"};
        schema["properties"] = {
            {"version", {{"type", "integer"}, {"minimum", 1}}},
            {"type",    {{"type", "string"}}},
            {"data", {
                {"type",       "object"},
                {"required",   dataRequired},
                {"properties", dataProps}
            }}
        };

        return schema;
    }

    // Builds a BinarySchema from the reflected TypeInfo of T.
    //
    // expectedTypeHash is computed via FNV1a on the type name so it always stays
    // in sync with the registered type name — no manual hash needed.
    //
    // minTotalBytes is sizeof(BinaryHeader) plus the sum of all fixed-size field
    // byte sizes. String fields are variable-length and contribute 0 to this floor,
    // so the actual minimum may be higher if strings are present.
    template<typename T>
    BinarySchema BinarySchemaFromType(const Version minVer, const Version maxVer = 0) {
        const TypeInfo& info = GetTypeInfo<T>();

        size_t payloadBytes = 0;
        for (const FieldInfo& field : info.fields)
            payloadBytes += Detail::FieldByteSize(field.type);

        return BinarySchema {
            FNV1a(info.typeName),
            minVer,
            maxVer,
            sizeof(BinaryHeader) + payloadBytes
        };
    }

} // namespace DotData
