cmake_minimum_required(VERSION 3.16)
project(DotData LANGUAGES CXX)

set(DOTDATA_SOURCES
    src/DotData.h
    src/Versioning/DataVersion.h
    src/Reflection/Reflect.h
    src/Serialization/JsonSerializer.h
    src/Serialization/BinarySerializer.h
    src/Validation/Validator.h
    src/Validation/SchemaFromReflection.h
    src/Validation/JsonSchemaValidator.h
    src/Validation/BinarySchemaValidator.h
    src/Storage/StorageLayer.h
    src/Storage/StorageLayer.cpp
    src/Storage/StoragePaths.h
)

add_library(DotData STATIC ${DOTDATA_SOURCES})

target_include_directories(DotData PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DotData PUBLIC
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
)

target_compile_features(DotData PUBLIC cxx_std_23)

if(MSVC)
    target_compile_options(DotData PRIVATE /W4)
else()
    target_compile_options(DotData PRIVATE -Wall -Wextra -Wpedantic)
endif()
