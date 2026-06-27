#pragma once

#include <chrono>
#include <format>
#include <string>

#if defined(DOTENGINE_PLATFORM_WINDOWS) || defined(DOTENGINE_PLATFORM_MACOS)

extern DotEngine::Application* DotEngine::createApplication();

int main(int /*argc*/, char** /*argv*/) {
    DotEngine::Log::Init();
    DOTENGINE_CORE_WARN("Initialized Log!");

    auto now  = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    std::string time{std::format("{:%d/%m/%Y %H:%M:%S}", now)};
    DOTENGINE_INFO("Hello! time: {0}", time.data());

    auto* app = DotEngine::createApplication();
    app->Run();
    delete app;

    return 0;
}

#elif defined(DOTENGINE_PLATFORM_LINUX)

int main(int /*argc*/, char** /*argv*/) {
    DotEngine::Log::Init();
    DOTENGINE_CORE_ERROR("Unsupported platform!");
    return 1;
}

#endif
