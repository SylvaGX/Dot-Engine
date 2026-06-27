#pragma once

// DotData — unified serialization, reflection, validation & persistence module.
//
// Include this header to access the full public API.
// Modules that only need a subset can include the sub-headers directly.

#include "Versioning/DataVersion.h"
#include "Reflection/Reflect.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/BinarySerializer.h"
#include "Validation/Validator.h"
#include "Validation/SchemaFromReflection.h"
#include "Validation/JsonSchemaValidator.h"
#include "Validation/BinarySchemaValidator.h"
#include "Storage/StorageLayer.h"
#include "Storage/StoragePaths.h"
