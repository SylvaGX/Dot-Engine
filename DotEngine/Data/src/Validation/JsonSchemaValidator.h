#pragma once

#include "Validation/Validator.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

namespace DotData {

    // Validates a parsed nlohmann::json document against a JSON Schema.
    //
    // Construct with a schema produced by BuildJsonSchema<T>() from SchemaFromReflection.h.
    // Call Validate() before deserializing fields to ensure structural correctness.
    //
    // Example:
    //   auto schema = BuildJsonSchema<WindowConfig>();
    //   JsonSchemaValidator v(std::move(schema));
    //   auto result = v.Validate(parsedJson);
    //   if (!result.valid) { /* reject file */ }
    class JsonSchemaValidator {
    public:
        explicit JsonSchemaValidator(nlohmann::json schema)
            : m_Validator(std::move(schema))
        {}

        [[nodiscard]] ValidationResult Validate(const nlohmann::json& doc) const {
            try {
                m_Validator.validate(doc);
            } catch (const std::exception& e) {
                return ValidationResult::Fail(e.what());
            }

            return ValidationResult::Ok();
        }

    private:
        nlohmann::json_schema::json_validator m_Validator;
    };

} // namespace DotData
