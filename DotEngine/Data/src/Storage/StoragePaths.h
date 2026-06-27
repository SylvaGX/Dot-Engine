#pragma once

#include <filesystem>

namespace DotData::Paths {

    // Returns %APPDATA%/DotEngine/ on Windows, $HOME/.dotengine/ on POSIX.
    std::filesystem::path EngineData();

    // Returns <projectRoot>/.dotdata/
    std::filesystem::path ProjectData(const std::filesystem::path& projectRoot);

} // namespace DotData::Paths
