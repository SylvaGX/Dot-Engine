#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace DotData {

    // Thin filesystem abstraction. Knows nothing about formats, versioning, or
    // serialization — it only moves bytes between memory and disk.
    class StorageLayer {
    public:
        // Writes UTF-8 text content to path, creating parent directories as needed.
        // Returns false on failure.
        /* TODO:
         * Improve this function, with writ batching with multi-thread to improve performance. Be careful just
         * how it is written since the data needs to be written sequential, so it has an order. If it is done by structs or block of data it can
         * help avoid that problem
         */
        static bool WriteText(const std::filesystem::path& path, std::string_view content);

        // Reads entire file into outContent.
        // Returns false if the file does not exist or cannot be read.
        /* TODO:
         * Improve this function also, with read batching so if it happens to have big size files, not to read all to memory.
         * Also possibly with the batching adding multi-thread for the batching for better performance. Be careful just
         * how it is read since the data needs to be read sequential, so it has an order. If it is done by structs or block of data it can
         * help avoid that problem
         */
        static bool ReadText(const std::filesystem::path& path, std::string& outContent);

        // Writes raw binary data to path, creating parent directories as needed.
        // Returns false on failure.
        /* TODO:
         * Same batching / multi-thread improvements as WriteText once binary assets grow large.
         */
        static bool WriteBytes(const std::filesystem::path& path, const std::vector<std::byte>& data);

        // Reads entire binary file into outData.
        // Returns false if the file does not exist or cannot be read.
        /* TODO:
         * Same batching improvements as ReadText for large binary assets.
         */
        static bool ReadBytes(const std::filesystem::path& path, std::vector<std::byte>& outData);

        /* TODO:
         * Check how this functions is used and if it guarantees that it happens in an atomic operation, so it avoids the TOCTOU vulnerability
         */
        static bool Exists(const std::filesystem::path& path);

        // Creates all missing directories in path (equivalent to mkdir -p).
        static bool EnsureDir(const std::filesystem::path& path);
    };

} // namespace DotData
