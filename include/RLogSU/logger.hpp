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

namespace RLSU {
    
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
        DUMP,       //                    logfile on_debug 
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

    size_t base_tabs_num = 0;
    std::string BaseTabsStr()
    {
        std::string tab = "";
        for (size_t i = 0; i < base_tabs_num; i++)
            tab += "  ";

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
        const std::string& log_folder = 
        #ifdef LOG_DIR
        (std::string(LOG_DIR).empty() ? "log" : std::string(LOG_DIR));
        #else
        "log";
        #endif
        
        GetLogfile_().open(log_folder + "/" + logfile_name_);

        return GetLogfile_().is_open();
    }

    );

    void ColoredLog_(LogLevel log_level, const std::string text, const std::string code_place_str);
};

static Logger ModuleLogger;
 

}   // namespace RLSU


#ifndef MODULE_NAME
#define MODULE_NAME  "unknown"
#endif

#define PZDC                                                                                    \
    do {                                                                                        \
    if (!RLSU::ModuleLogger.module_name_inited)                                                 \
    {                                                                                           \
        RLSU::ModuleLogger.module_name = MODULE_NAME;                                           \
        RLSU::ModuleLogger.module_name_inited = true;                                           \
    }                                                                                           \
    } while(0)                                                                                  \

#define RLSU_INFO(    std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::INFO   , std_format_, ##__VA_ARGS__);} while(0) 
#define RLSU_MESSAGE( std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::MESSAGE, std_format_, ##__VA_ARGS__);} while(0)
#define RLSU_LOG(     std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::LOG    , std_format_, ##__VA_ARGS__);} while(0)
#define RLSU_DUMP(    std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::DUMP   , std_format_, ##__VA_ARGS__);} while(0)
#define RLSU_WARNING( std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::WARNING, std_format_, ##__VA_ARGS__);} while(0)
#define RLSU_ERROR(   std_format_, ...) do {PZDC;  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::ERROR  , std_format_, ##__VA_ARGS__);} while(0)


#define RLSU_BASETAB_INCREACE RLSU::ModuleLogger.base_tabs_num++
#define RLSU_BASETAB_DECREACE RLSU::ModuleLogger.base_tabs_num--

#include "RLogSU/error_handler.hpp"