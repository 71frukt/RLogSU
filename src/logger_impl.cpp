#include "logger_impl.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ostream>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <fmt/chrono.h>

#include "logger_appearance.hpp"
#include "RLogSU/logger.hpp"


namespace RLSU::Log {

void Logger::SetLogSpace(const std::string& path)
{
    if (logfolder_is_initialized_.load(std::memory_order_acquire))
    {
        throw std::runtime_error("Redefinition of LogSpace after logger initialization");
    }

    std::lock_guard<std::mutex> lock(log_space_mutex_);
    log_space_ = path;
}

std::string Logger::GetLogSpace()
{
    std::lock_guard<std::mutex> lock(log_space_mutex_);
    return log_space_;
}

static std::string NowTimeToString()
{
    auto now = std::chrono::system_clock::now();
    
    return fmt::format("{:%Y_%m_%d_%H_%M_%S}", now);
}

bool Logger::LogFolderIsInited_()
{
    return logfolder_is_initialized_.load(std::memory_order_acquire);
}


void Logger::InitializeLogFolder_()
{
    log_folder_ = NowTimeToString();

    std::error_code ec;
    std::filesystem::create_directories(GetLogSpace() + "/" + log_folder_, ec);

    if (ec)
    {
        throw std::runtime_error("Couldn't create a logfile directory");
    }

    logfile_.open(GetLogSpace() + "/" + log_folder_ + "/" + std::string(logfile_name_));
    logfile_ << std::unitbuf;

    if (!logfile_)
        throw std::runtime_error("Faild opening of logfile");

    logfile_ << "<html>                                                                                                     \n"
             << "\t<head>                                                                                                   \n"
             << "\t<title>" << logfile_name_ << "</title>                                                                   \n"
             << "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
             << "\t</head>                                                                                                  \n"
             << "\t<body>                                                                                                   \n"
             << "\t<div class=\"jumbotron text-center\">                                                                    \n"
             << "\t\t<h1>" << logfile_name_ << "</h1>                                                                       \n"
             << "\t</div>                                                                                                   \n"
             << "\t<pre>                                                                                                    \n"
             << "\t<style>                                                                                                  \n"
             << Detail::Appearance::Html::COLORS_HTML_PREAMBLE() << Detail::Appearance::Html::TABLE_HTML_COLOR()
             << "\t</style>                                                                                                 \n";

    // std::cerr << "Opened Logfile" << std::endl;

    logfolder_is_initialized_.store(true, std::memory_order_release);
}

Logger::~Logger()
{
#ifndef NDEBUG
    if (logfile_.is_open())
    {
        logfile_ << "\t\t</pre> \n"
                    "\t</body>  \n"
                    "</html>";

        logfile_.close();
        // std::cerr << "Closed Logfile" << std::endl;
    }
#endif // !NDEBUG
}



void Logger::ColoredLog(LogLevel log_level, const std::string text, const std::string code_place)
{
    using namespace Detail::Appearance;

    switch (log_level)
    {
    case LogLevel::INFO:
        RLSU_ON_DEBUG(std::cerr << BaseTabsStr() << Console::ColoredStr(Console::CYAN,    "[INFO]       ") << text << std::endl;)
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::CYAN,    "[INFO]       ") << text << std::endl;)
        break;

    case LogLevel::MESSAGE:
                      std::cerr << BaseTabsStr() << Console::ColoredStr(Console::CYAN,    "[MESSAGE]    ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::CYAN, text) << std::endl;
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::CYAN,    "[MESSAGE]    ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::CYAN, text) << std::endl;)
        break;

    case LogLevel::LOG:
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << text;)
        break;

    case LogLevel::DUMP:
        RLSU_ON_DEBUG(std::cerr << BaseTabsStr() << Console::ColoredStr(Console::SKYBLUE, "[DUMP]       ") << Console::ColoredFormatedCodePlace(code_place) << text;)
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::SKYBLUE, "[DUMP]       ") << Html   ::ColoredFormatedCodePlace(code_place) << text;)
        break;

    case LogLevel::WARNING:
                      std::cerr << BaseTabsStr() << Console::ColoredStr(Console::YELLOW,  "[WARNING]    ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::YELLOW, text) << std::endl;
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::YELLOW,  "[WARNING]    ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::YELLOW, text) << std::endl;)
        break;

    case LogLevel::ERROR:
                      std::cerr << BaseTabsStr() << Console::ColoredStr(Console::RED,     "[ERROR]      ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::RED,   text) << std::endl;
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::RED,     "[ERROR]      ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::RED,   text) << std::endl;)
        break;

    case LogLevel::ASSERT:
                      std::cerr << BaseTabsStr() << Console::ColoredStr(Console::RED,     "[ASSERT]     ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::RED,   text) << std::endl;
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::RED,     "[ASSERT]     ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::RED,   text) << std::endl;)
        break;

    case LogLevel::EXCEPT_RUNTIME:
                      std::cerr << BaseTabsStr() << Console::ColoredStr(Console::MAGENTA, "[EXCEPTION]  ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::WHITE, text) << std::endl;
        RLSU_ON_DEBUG(logfile_  << BaseTabsStr() << Html   ::ColoredStr(Html   ::MAGENTA, "[EXCEPTION]  ") << Html   ::ColoredFormatedCodePlace(code_place) <<    Html::ColoredStr(Html   ::WHITE, text) << std::endl;)
        break;
    
    default:
        std::cerr << "Unknown Log level!\n";
        break;
    }

}


std::string GetRelativePath(const std::string& abs_path_str)
{
    std::filesystem::path abs_path(abs_path_str);
    std::filesystem::path current_path = std::filesystem::current_path();

    std::filesystem::path relative_path = std::filesystem::relative(abs_path, current_path);

    return relative_path.empty() ? abs_path_str : relative_path.string();
}


}   // namespace RLSU
