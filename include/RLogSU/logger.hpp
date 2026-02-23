#include "RLogSU/logger_impl.hpp"


#define     RLSU_WARNING( std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::WARNING, std_format_, ##__VA_ARGS__);} while(0)
#define     RLSU_ERROR(   std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::ERROR  , std_format_, ##__VA_ARGS__);} while(0)

#ifndef NDEBUG    
    #define RLSU_INFO(    std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::INFO   , std_format_, ##__VA_ARGS__);} while(0) 
    #define RLSU_MESSAGE( std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::MESSAGE, std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_LOG(     std_format_, ...) do {RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::LOG    , std_format_, ##__VA_ARGS__);} while(0)

    #define RLSU_DUMP(_DumpFuncCall, ...)                                                           \
    do {                                                                                            \
        RLSU::Log::UnitLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::DUMP, "\n" );    \
        __VA_OPT__(                                                                                 \
            RLSU_LOG("Dumping '{}'\n", __VA_ARGS__);                                                \
        )                                                                                           \
        _DumpFuncCall;                                                                              \
    } while(0)

#else
    #define RLSU_INFO(    std_format_, ...)
    #define RLSU_MESSAGE( std_format_, ...)
    #define RLSU_LOG(     std_format_, ...)
    #define RLSU_DUMP(_DumpFunc, ...)
#endif



#define RLSU_BASETAB_INCREACE RLSU::Log::UnitLogger.BaseTabsNum++
#define RLSU_BASETAB_DECREACE RLSU::Log::UnitLogger.BaseTabsNum--
