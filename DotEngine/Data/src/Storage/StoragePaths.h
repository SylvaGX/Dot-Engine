#pragma once

#include <filesystem>

namespace DotData::Paths {

    // Returns %APPDATA%/DotEngine/ on Windows,
    // ~/Library/Application Support/DotEngine/ on macOS,
    // $HOME/.dotengine/ on Linux.
    std::filesystem::path EngineData();

    // Returns <projectRoot>/.dotdata/
    std::filesystem::path ProjectData(const std::filesystem::path& projectRoot);

} // namespace DotData::Paths
