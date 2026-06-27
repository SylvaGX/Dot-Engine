#pragma once

#include "CoreTypes.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace DotEngine {

    class DOTENGINE_API Log {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger()   { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Core log macros
#define DOTENGINE_CORE_TRACE(...)    ::DotEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DOTENGINE_CORE_INFO(...)     ::DotEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DOTENGINE_CORE_WARN(...)     ::DotEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DOTENGINE_CORE_ERROR(...)    ::DotEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DOTENGINE_CORE_CRITICAL(...) ::DotEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DOTENGINE_TRACE(...)    ::DotEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DOTENGINE_INFO(...)     ::DotEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define DOTENGINE_WARN(...)     ::DotEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DOTENGINE_ERROR(...)    ::DotEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define DOTENGINE_CRITICAL(...) ::DotEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
