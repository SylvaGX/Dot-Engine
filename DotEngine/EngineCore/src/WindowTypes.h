#pragma once

#include "DotData.h"

#include <string>
#include <cstdint>

#include "Log.h"

namespace DotEngine {

    struct WindowData {
        std::string title        = "Dot Engine";
        uint32_t    width        = 1280;
        uint32_t    height       = 720;
        int32_t     x_position   = 0;
        int32_t     y_position   = 0;
        bool        maximized   = false;
        bool        vsync        = true;
        void*       nativeHandle = nullptr; // GLFWwindow* — not serialized
    };

}

DOTDATA_REFLECT_BEGIN(DotEngine::WindowData)
    DOTDATA_FIELD(DotEngine::WindowData, title,      DotData::FieldType::String)
    DOTDATA_FIELD(DotEngine::WindowData, width,      DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowData, height,     DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowData, x_position, DotData::FieldType::Int)
    DOTDATA_FIELD(DotEngine::WindowData, y_position, DotData::FieldType::Int)
    DOTDATA_FIELD(DotEngine::WindowData, maximized, DotData::FieldType::Bool)
    DOTDATA_FIELD(DotEngine::WindowData, vsync,      DotData::FieldType::Bool)
DOTDATA_REFLECT_END()
