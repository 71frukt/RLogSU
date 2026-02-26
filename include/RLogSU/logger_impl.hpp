#pragma once

#include <cstddef>
#include <fstream>
#include <cassert>
#include <fmt/core.h>
#include <string>

#include <mutex>
#include <atomic>

#ifndef NDEBUG
#define RLSU_ON_DEBUG(code_)  code_
#else
#define RLSU_ON_DEBUG(code_)
#endif


namespace RLSU::Log {
    

std::string GetRelativePath(const std::string& abs_path_str);


class Logger
{
public:

    ~Logger();

    enum LogLevel {
        INFO,               // console on_debug
        MESSAGE,            // console always   + logfile on_debug
        LOG,                // console always   + logfile on_debug
        DUMP,               // console on_debug + logfile on_debug 
        WARNING,            // console always   + logfile on_debug
        ERROR,              // console always   + logfile on_debug
        EXCEPT_RUNTIME      // console always   + logfile on_debug
        // VERIFY,     // console always   + logfile on_debug
    };

    template<typename... Args>
    void Log(const char *const file, const int line, const char *const func, LogLevel log_level, const std::string& format, Args&&... args)
    {
        assert(file);
        assert(func);

        #ifndef NDEBUG
            std::call_once(logfolder_init_flag_, &Logger::InitializeLogFolder_, this);
        #endif
    
        std::string message    = fmt::vformat(format, fmt::make_format_args(std::forward<Args>(args)...));
        std::string code_place = fmt::format("{}:{}({})", GetRelativePath(file), line, func);

        std::lock_guard<std::mutex> lock(write_mutex_);

        ColoredLog(log_level, message, code_place);
    }

    void ColoredLog(LogLevel log_level, const std::string text, const std::string code_place_str);

    #ifndef NDEBUG
        void        SetLogSpace(const std::string& path);
        std::string GetLogSpace();

        std::string GetLogsFolder()
        {
            std::call_once(logfolder_init_flag_, &Logger::InitializeLogFolder_, this);
            return log_folder_;
        }

    #endif

    size_t BaseTabsNum = 0;

    std::string BaseTabsStr()
    {
        std::string tab = "";
        for (size_t i = 0; i < BaseTabsNum; i++)
            tab += "    ";

        return tab;
    }

private:

    std::mutex     write_mutex_;

    #ifndef NDEBUG
    static constexpr std::string_view logfile_name_ = "Logfile.html";
        
        std::ofstream logfile_;

        std::once_flag logfolder_init_flag_;

        std::atomic<bool> logfolder_is_initialized_ = false;

        std::string       log_space_ = "logs";
        std::mutex        log_space_mutex_;

        std::string       log_folder_ = "hz_logs";


        void InitializeLogFolder_();
        bool LogFolderIsInited_();
    #endif // !NDEBUG

};


inline Logger UnitLogger;


}   // namespace RLSU::Log
