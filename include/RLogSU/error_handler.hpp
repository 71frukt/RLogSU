#pragma once

namespace RLSU {

#define ERROR_HANDLE(_CallFunc)                                                                                                 \
    [&]() -> decltype(auto) {                                                                                                   \
        try {                                                                                                                   \
            return (_CallFunc);                                                                                                 \
        }                                                                                                                       \
        catch (const std::runtime_error& _e) {                                                                                  \
            RLSU_ERROR("Call failed: '{}'  what(): {}", #_CallFunc, _e.what());                                                 \
            throw;                                                                                                              \
        }                                                                                                                       \
    }()


// ... = exit_val that compares the return value of the function
#define RLSU_VERIFY(_condition, ...)                                                                                            \
do {                                                                                                                            \
    if (!(_condition))                                                                                                          \
    {                                                                                                                           \
        PZDC;                                                                                                                   \
        RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::VERIFY, std::string(#_condition));                   \
        __VA_OPT__(                                                                                                             \
            RLSU_MESSAGE(__VA_ARGS__);                                                                                          \
            RLSU_LOG("\n");                                                                                                     \
        )                                                                                                                       \
        throw std::runtime_error("verification failed");                                                                        \
    }                                                                                                                           \
} while(0)



#ifdef DEBUG_MODE

#define ASSERT_HANDLE(_CallFunc)                                                                                                \
    [&]() -> decltype(auto) {                                                                                                   \
        try {                                                                                                                   \
            return (_CallFunc);                                                                                                 \
        }                                                                                                                       \
        catch (const std::runtime_error& _e) {                                                                                  \
            RLSU_ERROR("verificator failed: '{}'  what(): {}", #_CallFunc, _e.what());                                          \
            throw;                                                                                                              \
        }                                                                                                                       \
    }()


#define RLSU_ASSERT(_condition, ...)                                                                                            \
do {                                                                                                                            \
    if (!(_condition))                                                                                                          \
    {                                                                                                                           \
        PZDC;                                                                                                                   \
        RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::ASSERT, std::string(#_condition));                   \
        __VA_OPT__(                                                                                                             \
            RLSU_MESSAGE(__VA_ARGS__);                                                                                          \
            RLSU_LOG("\n");                                                                                                     \
        )                                                                                                                       \
        throw std::runtime_error("assertion failed");                                                                           \
    }                                                                                                                           \
} while(0)

#define RLSU_DUMP(_DumpFunc)                                                                                                    \
do {                                                                                                                            \
    PZDC;                                                                                                                       \
    RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::DUMP, "\n" );                                              \
    _DumpFunc;                                                                                                                  \
} while(0)


#else
#define ASSERT_HANDLE(_CallFunc)
#define RLSU_ASSERT(_condition, ...)
#define RLSU_DUMP(_DumpFunc)
#endif

} // namespace RLSU