#include "StorageLayer.h"
#include "StoragePaths.h"

#include <fstream>
#include <iterator>
#include <sstream>
#include <system_error>

namespace DotData {

    bool StorageLayer::WriteText(const std::filesystem::path& path, std::string_view content) {
        std::error_code ec;
        std::filesystem::create_directories(path.parent_path(), ec);
        if (ec) return false;

        std::ofstream file(path, std::ios::out | std::ios::trunc);
        if (!file.is_open()) return false;

        file << content;
        return file.good();
    }

    bool StorageLayer::ReadText(const std::filesystem::path& path, std::string& outContent) {
        std::ifstream file(path, std::ios::in);
        if (!file.is_open()) return false;

        std::ostringstream ss;
        ss << file.rdbuf();
        outContent = ss.str();
        return true;
    }

    bool StorageLayer::WriteBytes(const std::filesystem::path& path, const std::vector<std::byte>& data) {
        std::error_code ec;
        std::filesystem::create_directories(path.parent_path(), ec);
        if (ec) return false;

        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) return false;

        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        return file.good();
    }

    bool StorageLayer::ReadBytes(const std::filesystem::path& path, std::vector<std::byte>& outData) {
        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open()) return false;

        const auto size = file.tellg();
        if (size <= 0) return false;

        file.seekg(0, std::ios::beg);
        outData.resize(static_cast<size_t>(size));
        file.read(reinterpret_cast<char*>(outData.data()), size);
        return file.good();
    }

    bool StorageLayer::Exists(const std::filesystem::path& path) {
        std::error_code ec;
        return std::filesystem::exists(path, ec);
    }

    bool StorageLayer::EnsureDir(const std::filesystem::path& path) {
        std::error_code ec;
        std::filesystem::create_directories(path, ec);
        return !ec;
    }

} // namespace DotData

// ---------------------------------------------------------------------------

namespace DotData::Paths {

    std::filesystem::path EngineData() {
#if defined(_WIN32)
        const char* appdata = std::getenv("APPDATA");
        if (appdata)
            return std::filesystem::path(appdata) / "DotEngine";
        return std::filesystem::temp_directory_path() / "DotEngine";
#else
        const char* home = std::getenv("HOME");
        if (home)
            return std::filesystem::path(home) / ".dotengine";
        return std::filesystem::temp_directory_path() / ".dotengine";
#endif
    }

    std::filesystem::path ProjectData(const std::filesystem::path& projectRoot) {
        return projectRoot / ".dotdata";
    }

} // namespace DotData::Paths
