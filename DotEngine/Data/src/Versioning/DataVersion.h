#pragma once

#include <cstdint>
#include <string_view>

namespace DotData {

    using Version = uint32_t;

    constexpr Version INVALID_VERSION = 0;

    // Every JSON file managed by DotData must use this envelope:
    //   { "version": N, "type": "TypeName", "data": { ... } }
    //
    // Every binary file managed by DotData must begin with this header layout:
    //   [4 bytes] Magic number  (DOTD = 0x444F5444)
    //   [4 bytes] Format version
    //   [4 bytes] Data type hash (FNV-1a of the type name string)
    //   [payload ...]

    constexpr uint32_t BINARY_MAGIC = 0x444F5444; // 'D','O','T','D'

    struct BinaryHeader {
        uint32_t magic   = BINARY_MAGIC;
        Version  version = INVALID_VERSION;
        uint32_t typeHash = 0;
    };

    // Compile-time FNV-1a hash for type name strings used in BinaryHeader::typeHash.
    constexpr uint32_t FNV1a(std::string_view str) noexcept {
        uint32_t hash = 2166136261u;
        for (char c : str) {
            hash ^= static_cast<uint32_t>(c);
            hash *= 16777619u;
        }
        return hash;
    }

} // namespace DotData
