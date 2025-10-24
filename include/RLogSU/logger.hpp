#pragma once

#include <cstddef>
#include <iostream>
#include <fstream>
#include <cassert>
#include <fmt/core.h>
#include <string>


#ifdef DEBUG_MODE
#define RLSU_ON_DEBUG(code_)  code_
#else
#define RLSU_ON_DEBUG(code_)
#endif

namespace RLSU::Log {
    
std::string GetRelativePath(const std::string& abs_path_str);


class Logger
{
public:
    explicit Logger();
    ~Logger();

    enum LogLevel {
        INFO,       // console on_debug
        MESSAGE,    // console always   + logfile on_debug
        LOG,        // console always   + logfile on_debug
        DUMP,       // console on_debug + logfile on_debug 
        WARNING,    // console always   + logfile on_debug
        ERROR,      // console always   + logfile on_debug
        VERIFY,     // console always   + logfile on_debug
        ASSERT      // console always   + logfile on_debug
    };

    template<typename... Args>
    void Log(const char *const file, const int line, const char *const func, LogLevel log_level, const std::string& format, Args&&... args)
    {
        assert(file);
        assert(func);

        std::string message    = fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...));
        std::string code_place = fmt::format("{}:{}({})", GetRelativePath(file), line, func);

        ColoredLog_(log_level, message, code_place);
    }

    std::string module_name;
    bool module_name_inited = false;

    static std::string GetLogFolder()
    {
        #ifdef LOG_DIR
        return (std::string(LOG_DIR).empty() ? "log" : std::string(LOG_DIR));
        #else
        return "log";
        #endif
    }

    static size_t BaseTabsNum;

    std::string BaseTabsStr()
    {
        std::string tab = "";
        for (size_t i = 0; i < BaseTabsNum; i++)
            tab += "    ";

        return tab;
    }

private:
    RLSU_ON_DEBUG(

    static std::ofstream& GetLogfile_()
    {
        static std::ofstream logfile;
        return logfile;
    }
    
    static constexpr std::string logfile_name_ = "Logfile.html";

    static bool InitializeLogfile_()
    {
        GetLogfile_().open(GetLogFolder() + "/" + logfile_name_);
        GetLogfile_() << std::unitbuf;

        return GetLogfile_().is_open();
    }

    );

    void ColoredLog_(LogLevel log_level, const std::string text, const std::string code_place_str);
};

static Logger ModuleLogger;


}   // namespace RLSU::Log


#ifndef MODULE_NAME
#define MODULE_NAME  "unknown"
#endif

#define PZDC                                                                                    \
    do {                                                                                        \
    if (!RLSU::Log::ModuleLogger.module_name_inited)                                            \
    {                                                                                           \
        RLSU::Log::ModuleLogger.module_name = MODULE_NAME;                                      \
        RLSU::Log::ModuleLogger.module_name_inited = true;                                      \
    }                                                                                           \
    } while(0)                                                                                  \

    
    
    
#ifdef DEBUG_MODE    
    #define RLSU_WARNING( std_format_, ...) do {PZDC;  RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::WARNING, std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_ERROR(   std_format_, ...) do {PZDC;  RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::ERROR  , std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_INFO(    std_format_, ...) do {PZDC;  RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::INFO   , std_format_, ##__VA_ARGS__);} while(0) 
    #define RLSU_MESSAGE( std_format_, ...) do {PZDC;  RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::MESSAGE, std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_LOG(     std_format_, ...) do {PZDC;  RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::LOG    , std_format_, ##__VA_ARGS__);} while(0)

    #define RLSU_DUMP(_DumpFunc, ...)                                                               \
    do {                                                                                            \
        PZDC;                                                                                       \
        RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::DUMP, "\n" );  \
        __VA_OPT__(                                                                                 \
            RLSU_LOG("Dumping '{}'\n", __VA_ARGS__);                                                \
        )                                                                                           \
        _DumpFunc;                                                                                  \
    } while(0)

#else
    #define RLSU_WARNING( std_format_, ...) do {RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::WARNING, std_format_, ##__VA_ARGS__);} while(0)
    #define RLSU_ERROR(   std_format_, ...) do {RLSU::Log::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Log::Logger::ERROR  , std_format_, ##__VA_ARGS__);} while(0)

    #define RLSU_INFO(    std_format_, ...)
    #define RLSU_MESSAGE( std_format_, ...)
    #define RLSU_LOG(     std_format_, ...)
    #define RLSU_DUMP(_DumpFunc, ...)
#endif



#define RLSU_BASETAB_INCREACE RLSU::Log::ModuleLogger.BaseTabsNum++
#define RLSU_BASETAB_DECREACE RLSU::Log::ModuleLogger.BaseTabsNum--

#include "RLogSU/error_handler.hpp"