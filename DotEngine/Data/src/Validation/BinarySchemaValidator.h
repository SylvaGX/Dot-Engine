#pragma once

#include "Validation/Validator.h"
#include "Versioning/DataVersion.h"

#include <cstddef>
#include <cstring>
#include <span>
#include <string>

namespace DotData {

    // Describes the expected shape of a DotData binary file for a specific type.
    // Construct via BinarySchemaFromType<T>() in SchemaFromReflection.h.
    struct BinarySchema {
        uint32_t expectedTypeHash;  // FNV1a of the registered type name
        Version  minVersion;
        Version  maxVersion;        // 0 = no upper bound
        size_t   minTotalBytes;     // sizeof(BinaryHeader) + fixed-size field bytes
    };

    // Validates a raw binary buffer against a BinarySchema before payload parsing.
    //
    // Checks (in order):
    //   1. Buffer is large enough to hold a BinaryHeader
    //   2. magic == BINARY_MAGIC ('D','O','T','D')
    //   3. version is within [minVersion, maxVersion] (maxVersion == 0 means unbounded)
    //   4. typeHash matches expectedTypeHash
    //   5. Total buffer size >= minTotalBytes
    //
    // Example:
    //   auto schema = BinarySchemaFromType<MeshData>(1);
    //   BinarySchemaValidator v(schema);
    //   auto result = v.Validate(fileBytes);
    //   if (!result.valid) { /* reject file */ }
    class BinarySchemaValidator {
    public:
        explicit BinarySchemaValidator(BinarySchema schema)
            : m_Schema(schema)
        {}

        [[nodiscard]] ValidationResult Validate(const std::span<const std::byte> buf) const {
            if (buf.size() < sizeof(BinaryHeader))
                return ValidationResult::Fail("binary file too small to contain a header");

            BinaryHeader header;
            std::memcpy(&header, buf.data(), sizeof(BinaryHeader));

            if (header.magic != BINARY_MAGIC)
                return ValidationResult::Fail("invalid magic number — not a DotData binary file");

            if (header.version == INVALID_VERSION)
                return ValidationResult::Fail("binary file has invalid version (0)");

            if (header.version < m_Schema.minVersion)
                return ValidationResult::Fail(
                    "binary file version " + std::to_string(header.version) +
                    " is older than minimum supported version " + std::to_string(m_Schema.minVersion));

            if (m_Schema.maxVersion != 0 && header.version > m_Schema.maxVersion)
                return ValidationResult::Fail(
                    "binary file version " + std::to_string(header.version) +
                    " is newer than maximum supported version " + std::to_string(m_Schema.maxVersion));

            if (header.typeHash != m_Schema.expectedTypeHash)
                return ValidationResult::Fail("binary file type hash mismatch — wrong type or corrupted header");

            if (buf.size() < m_Schema.minTotalBytes)
                return ValidationResult::Fail(
                    "binary file payload is too small: " + std::to_string(buf.size()) +
                    " bytes, expected at least " + std::to_string(m_Schema.minTotalBytes));

            return ValidationResult::Ok();
        }

    private:
        BinarySchema m_Schema;
    };

} // namespace DotData
