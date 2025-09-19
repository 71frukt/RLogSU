#pragma once

namespace RLSU {

// ... = exit_val returning if func fails
#define ERROR_HANDLE(_CallFunc)                                                     \
    [&]() -> decltype(auto) {                                                       \
        try {                                                                       \
            return (_CallFunc);                                                     \
        }                                                                           \
        catch (const std::runtime_error& _e) {                                      \
            RLSU_ERROR("Call failed: '{}'  what(): {}", #_CallFunc, _e.what());     \
            throw;                                                                  \
        }                                                                           \
    }()

// ... = exit_val that compares the return value of the function
#define RLSU_VERIFY(_condition, ...)                                                                                            \
do {                                                                                                                            \
    if (!(_condition))                                                                                                          \
    {                                                                                                                           \
        RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::VERIFY, std::string(#_condition));                   \
        if (#__VA_ARGS__[0] != 0)                                                                                               \
        {                                                                                                                       \
            RLSU_MESSAGE("" __VA_OPT__(__VA_ARGS__));                                                                           \
            RLSU_LOG("\n");                                                                                                     \
        }                                                                                                                       \
        throw std::runtime_error("verification failed");                                                                        \
    }                                                                                                                           \
} while(0)


#define RLSU_ASSERT(_condition, ...)                                                                                            \
do {                                                                                                                            \
    if (!(_condition))                                                                                                          \
    {                                                                                                                           \
        RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::ASSERT, std::string(#_condition));                   \
        __VA_OPT__(                                                                                                             \
            RLSU_MESSAGE(__VA_ARGS__);                                                                                          \
            RLSU_LOG("\n");                                                                                                     \
        )                                                                                                                       \
        throw std::runtime_error("assertion failed");                                                                           \
    }                                                                                                                           \
} while(0)

}