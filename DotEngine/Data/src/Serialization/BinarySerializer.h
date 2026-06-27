#pragma once

#include "Reflection/Reflect.h"
#include "Versioning/DataVersion.h"
#include "Validation/BinarySchemaValidator.h"
#include "Validation/SchemaFromReflection.h"
#include "Storage/StorageLayer.h"

#include <cstddef>
#include <cstring>
#include <filesystem>
#include <span>
#include <string>
#include <vector>

namespace DotData {

    // Reflection-driven binary serializer.
    //
    // Every file written by Save() follows the mandatory DotData binary layout:
    //   [sizeof(BinaryHeader)] BinaryHeader  { magic, version, typeHash }
    //   [payload]              Reflected fields written in registration order
    //
    // String fields are serialized as a 4-byte length prefix followed by UTF-8 bytes.
    //
    // Load() validates the header via BinarySchemaValidator before parsing the payload,
    // ensuring the magic, version, and type hash are all correct before any data is read.
    class BinarySerializer {
    public:

        template<typename T>
        static bool Save(const T& obj,
                         const std::filesystem::path& path,
                         Version version)
        {
            if (version == INVALID_VERSION) return false;

            const TypeInfo& info = GetTypeInfo<T>();

            std::vector<std::byte> buf;
            buf.reserve(sizeof(BinaryHeader) + 256);

            BinaryHeader header;
            header.magic    = BINARY_MAGIC;
            header.version  = version;
            header.typeHash = FNV1a(info.typeName);

            const auto* headerBytes = reinterpret_cast<const std::byte*>(&header);
            buf.insert(buf.end(), headerBytes, headerBytes + sizeof(BinaryHeader));

            for (const FieldInfo& field : info.fields) {
                const void* ptr = static_cast<const char*>(static_cast<const void*>(&obj)) + field.offset;
                AppendField(buf, ptr, field.type);
            }

            return StorageLayer::WriteBytes(path, buf);
        }

        template<typename T>
        static bool Load(T& obj,
                         const std::filesystem::path& path,
                         Version& outVersion)
        {
            std::vector<std::byte> buf;
            if (!StorageLayer::ReadBytes(path, buf)) return false;

            {
                const BinarySchemaValidator validator(BinarySchemaFromType<T>(1));
                if (auto [valid, error] = validator.Validate(std::span<const std::byte>(buf)); !valid) return false;
            }

            BinaryHeader header;
            std::memcpy(&header, buf.data(), sizeof(BinaryHeader));
            outVersion = header.version;

            const TypeInfo& info  = GetTypeInfo<T>();
            size_t          offset = sizeof(BinaryHeader);

            for (const FieldInfo& field : info.fields) {
                void* ptr = static_cast<char*>(static_cast<void*>(&obj)) + field.offset;
                if (!ReadField(buf, offset, ptr, field.type)) return false;
            }

            return true;
        }

    private:

        static void AppendField(std::vector<std::byte>& buf, const void* ptr, const FieldType type) {
            switch (type) {
                case FieldType::Int: {
                    auto v = *static_cast<const int*>(ptr);
                    Append(buf, &v, sizeof(v));
                    break;
                }
                case FieldType::UInt32: {
                    auto v = *static_cast<const uint32_t*>(ptr);
                    Append(buf, &v, sizeof(v));
                    break;
                }
                case FieldType::Float: {
                    auto v = *static_cast<const float*>(ptr);
                    Append(buf, &v, sizeof(v));
                    break;
                }
                case FieldType::Double: {
                    auto v = *static_cast<const double*>(ptr);
                    Append(buf, &v, sizeof(v));
                    break;
                }
                case FieldType::Bool: {
                    auto v = *static_cast<const bool*>(ptr);
                    Append(buf, &v, sizeof(v));
                    break;
                }
                case FieldType::String: {
                    const auto& str = *static_cast<const std::string*>(ptr);
                    const auto len = static_cast<uint32_t>(str.size());
                    Append(buf, &len, sizeof(len));
                    Append(buf, str.data(), str.size());
                    break;
                }
            }
        }

        static bool ReadField(const std::vector<std::byte>& buf, size_t& offset, void* ptr, const FieldType type) {
            switch (type) {
                case FieldType::Int:    return Read(buf, offset, ptr, sizeof(int));
                case FieldType::UInt32: return Read(buf, offset, ptr, sizeof(uint32_t));
                case FieldType::Float:  return Read(buf, offset, ptr, sizeof(float));
                case FieldType::Double: return Read(buf, offset, ptr, sizeof(double));
                case FieldType::Bool:   return Read(buf, offset, ptr, sizeof(bool));
                case FieldType::String: {
                    uint32_t len = 0;
                    if (!Read(buf, offset, &len, sizeof(len))) return false;
                    if (offset + len > buf.size()) return false;
                    *static_cast<std::string*>(ptr) = std::string(
                        reinterpret_cast<const char*>(buf.data() + offset), len);
                    offset += len;
                    return true;
                }
            }
            return false;
        }

        static void Append(std::vector<std::byte>& buf, const void* src, const size_t n) {
            const auto* bytes = reinterpret_cast<const std::byte*>(src);
            buf.insert(buf.end(), bytes, bytes + n);
        }

        static bool Read(const std::vector<std::byte>& buf, size_t& offset, void* dst, const size_t n) {
            if (offset + n > buf.size()) return false;
            std::memcpy(dst, buf.data() + offset, n);
            offset += n;
            return true;
        }
    };

} // namespace DotData
