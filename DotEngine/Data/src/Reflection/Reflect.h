#pragma once

#include <string_view>
#include <vector>
#include <cstddef>

namespace DotData {

    enum class FieldType {
        Int,
        UInt32,
        Float,
        Double,
        Bool,
        String
    };

    struct FieldInfo {
        std::string_view name;
        FieldType        type;
        size_t           offset;
    };

    struct TypeInfo {
        std::string_view        typeName;
        std::vector<FieldInfo>  fields;
    };

    // Specialize this for each reflected type.
    // Use the DOTDATA_REFLECT_* macros below to generate specializations.
    template<typename T>
    const TypeInfo& GetTypeInfo();

} // namespace DotData

// ---------------------------------------------------------------------------
// Reflection registration macros
//
// Usage (in a .cpp or at namespace scope in a header):
//
//   DOTDATA_REFLECT_BEGIN(MyStruct)
//       DOTDATA_FIELD(MyStruct, myInt,    DotData::FieldType::Int)
//       DOTDATA_FIELD(MyStruct, myBool,   DotData::FieldType::Bool)
//       DOTDATA_FIELD(MyStruct, myString, DotData::FieldType::String)
//   DOTDATA_REFLECT_END()
// ---------------------------------------------------------------------------

#define DOTDATA_REFLECT_BEGIN(TypeName)                                  \
    template<>                                                           \
    inline const DotData::TypeInfo& DotData::GetTypeInfo<TypeName>() {  \
        static DotData::TypeInfo info {                                  \
            #TypeName,                                                   \
            {

#define DOTDATA_FIELD(TypeName, FieldName, FieldTypeVal)                 \
                DotData::FieldInfo {                                     \
                    #FieldName,                                          \
                    FieldTypeVal,                                        \
                    offsetof(TypeName, FieldName)                        \
                },

#define DOTDATA_REFLECT_END()                                            \
            }                                                            \
        };                                                               \
        return info;                                                     \
    }
