#pragma once

#include "DotData.h"

#include <string>
#include <cstdint>

namespace DotEngine {

    struct WindowProps {
        std::string title;
        uint32_t    width;
        uint32_t    height;
        bool        fullscreen = false;

        explicit WindowProps(const std::string& t = "Dot Engine",
                             uint32_t w           = 1280,
                             uint32_t h           = 720,
                             bool     fscreen     = false)
            : title(t), width(w), height(h), fullscreen(fscreen) {}
    };

}

DOTDATA_REFLECT_BEGIN(DotEngine::WindowProps)
    DOTDATA_FIELD(DotEngine::WindowProps, title,      DotData::FieldType::String)
    DOTDATA_FIELD(DotEngine::WindowProps, width,      DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowProps, height,     DotData::FieldType::UInt32)
    DOTDATA_FIELD(DotEngine::WindowProps, fullscreen, DotData::FieldType::Bool)
DOTDATA_REFLECT_END()
