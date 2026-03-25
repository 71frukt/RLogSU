#pragma once

#include <exception>
#include <source_location>
#include <string>

#include "../../src/logger_impl.hpp"

template <typename T>
concept StringConstructibleException = 
    std::derived_from<T, std::exception> &&
    std::constructible_from<T, std::string>;

template <StringConstructibleException ExceptT>
inline void RLSU_THROW(std::string message, std::source_location loc = std::source_location::current())
{
    std::string code_place = fmt::format("{}:{}({})", RLSU::Log::GetRelativePath(loc.file_name()), loc.line(), loc.function_name());
    throw ExceptT("\n[runtime err][" + code_place + "] " + message);
}

#define RLSU_LOG_RUNTIME_ERR(exception) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::EXCEPT_RUNTIME, e.what());} while(0)

#define     RLSU_WARNING( std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::WARNING, std_format_, ##__VA_ARGS__);} while(0)
#define     RLSU_ERROR(   std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::ERROR  , std_format_, ##__VA_ARGS__);} while(0)

#ifndef NDEBUG    
    #define RLSU_INFO(    std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::INFO   , std_format_, ##__VA_ARGS__);} while(0) 
    #define RLSU_MESSAGE( std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::MESSAGE, std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_LOG(     std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::LOG    , std_format_, ##__VA_ARGS__);} while(0)

    #define RLSU_ASSERT(condition, std_format_, ...) do {                                                                                              \
        if (!(condition)) {                                                                                                                              \
            RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::ASSERT , #condition " failed. " std_format_, ##__VA_ARGS__);    \
            abort();                                                                                                                                   \
        }                                                                                                                                              \
    } while(0)

    #define RLSU_DUMP(_DumpFuncCall, ...)                                                           \
    do {                                                                                            \
        RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::DUMP, "\n" );    \
        __VA_OPT__(                                                                                 \
            RLSU_LOG("Dumping '{}'\n", __VA_ARGS__);                                                \
        )                                                                                           \
        _DumpFuncCall;                                                                              \
    } while(0)

    #define RLSU_SET_LOGSPACE(path_str) do {RLSU::Log::UnitLogger.SetLogSpace(path_str); } while(0)

#else
    #define RLSU_INFO(    std_format_, ...)
    #define RLSU_MESSAGE( std_format_, ...)
    #define RLSU_LOG(     std_format_, ...)
    #define RLSU_ASSERT(condition, std_format_, ...)
    #define RLSU_DUMP(_DumpFunc, ...)
    #define RLSU_SET_LOGSPACE(path_str)
#endif

#define RLSU_FORMAT(std_format_, ...)  (fmt::format(std_format_, ##__VA_ARGS__))

#define RLSU_BASETAB_INCREACE RLSU::Log::UnitLogger.BaseTabsNum++
#define RLSU_BASETAB_DECREACE RLSU::Log::UnitLogger.BaseTabsNum--
