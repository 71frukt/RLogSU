#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <format>


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
    explicit Logger(const std::string& logfile_name);
    Logger() : Logger("Logfile.html") {} ;
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

        std::string message    = std::vformat(std::string_view(format), std::make_format_args(args...));
        std::string code_place = std::format("{}:{}({})", GetRelativePath(file), line, func);

        ColoredLog_(log_level, message, code_place);
    }

private:
    RLSU_ON_DEBUG(std::ofstream logfile_);
    
    #ifdef LOG_DIR
    const std::string log_folder_ = (std::string(LOG_DIR).empty() ? "log" : LOG_DIR)
    #else
    const std::string log_folder_ = "log";
    #endif

    #ifdef MODULE_NAME
    const std::string module_name_ = (std::string(MODULE_NAME).empty() ? "some_module" : MODULE_NAME);
    #else
    const std::string module_name_ = "some_module";
    #endif

    void ColoredLog_(LogLevel log_level, const std::string text, const std::string code_place_str);
};

RLSU::Logger& GetLogger();


static Logger ModuleLogger;

}   // namespace RLSU


#define RLSU_INFO(     std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::INFO   , std_format_, ##__VA_ARGS__)
#define RLSU_MESSAGE(  std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::MESSAGE, std_format_, ##__VA_ARGS__)
#define RLSU_LOG(      std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::LOG    , std_format_, ##__VA_ARGS__)
#define RLSU_DUMP(     std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::DUMP   , std_format_, ##__VA_ARGS__)
#define RLSU_WARNING(  std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::WARNING, std_format_, ##__VA_ARGS__)
#define RLSU_ERROR(    std_format_, ...)  RLSU::ModuleLogger.Log(__FILE__, __LINE__, __func__, RLSU::Logger::ERROR  , std_format_, ##__VA_ARGS__)
