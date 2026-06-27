#pragma once

#ifdef DOTENGINE_PLATFORM_WINDOWS
    #if DOTENGINE_DYNAMIC_LINK
        #ifdef DOTENGINE_BUILD_DLL
            #define DOTENGINE_API __declspec(dllexport)
        #else
            #define DOTENGINE_API __declspec(dllimport)
        #endif
    #else
        #define DOTENGINE_API
    #endif
    #define DOTENGINE_DEBUGBREAK() __debugbreak()
#elif defined(DOTENGINE_PLATFORM_MACOS) || defined(DOTENGINE_PLATFORM_LINUX)
    #define DOTENGINE_API
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_debugtrap)
            #define DOTENGINE_DEBUGBREAK() __builtin_debugtrap()
        #else
            #include <signal.h>
            #define DOTENGINE_DEBUGBREAK() raise(SIGTRAP)
        #endif
    #else
        #include <signal.h>
        #define DOTENGINE_DEBUGBREAK() raise(SIGTRAP)
    #endif
#else
    #error Unsupported platform!
#endif

#ifdef DOTENGINE_DEBUG
    #define DOTENGINE_ENABLE_ASSERTS
#endif

#ifdef DOTENGINE_ENABLE_ASSERTS
    #define DOTENGINE_ASSERT(x, ...)      { if(!(x)){ DOTENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__);      DOTENGINE_DEBUGBREAK(); } }
    #define DOTENGINE_CORE_ASSERT(x, ...) { if(!(x)){ DOTENGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DOTENGINE_DEBUGBREAK(); } }
#else
    #define DOTENGINE_ASSERT(x, ...)
    #define DOTENGINE_CORE_ASSERT(x, ...)
#endif

#define BIT(X) (1 << (X))
#define DOTENGINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
