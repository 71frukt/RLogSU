#include <cstdio>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ostream>
#include <string>
#include <filesystem>


#include "RLogSU/logger_appearance.hpp"
#include "RLogSU/logger.hpp"

namespace RLSU {


Logger::Logger()
{
    RLSU_ON_DEBUG(

        static bool is_opened = false;

    if (is_opened) return;

    InitializeLogfile_();

    if (!GetLogfile_())
        std::cout << "Faild opening of logfile";


    GetLogfile_() << "<html>                                                                                                     \n"
            << "\t<head>                                                                                                   \n"
            << "\t<title>" << logfile_name_ << "</title>                                                                    \n"
            << "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
            << "\t</head>                                                                                                  \n"
            << "\t<body>                                                                                                   \n"
            << "\t<div class=\"jumbotron text-center\">                                                                    \n"
            << "\t\t<h1>" << logfile_name_ << "</h1>                                                                        \n"
            << "\t</div>                                                                                                   \n"
            << "\t<pre>                                                                                                    \n"
            << "\t<style>                                                                                                  \n"
            << RLSU::Appearance::Html::COLORS_HTML_PREAMBLE() << RLSU::Appearance::Html::TABLE_HTML_COLOR()
            << "\t</style>                                                                                                 \n";
    
    std::cout << "Opened Logfile" << std::endl;

    is_opened = true;
    )
}


Logger::~Logger()
{
    RLSU_ON_DEBUG (

    static bool is_closed = false;

    if (is_closed)  return;

    GetLogfile_() << "\t\t</pre> \n"
                "\t</body>  \n"
                "</html>";

    GetLogfile_().close();

    std::cout << "Closed Logfile" << std::endl;
    is_closed = true;
    
    )
}



void Logger::ColoredLog_(LogLevel log_level, const std::string text, const std::string code_place)
{
    using namespace RLSU::Appearance;

    switch (log_level)
    {
    case LogLevel::INFO:
        RLSU_ON_DEBUG(std::cout      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::CYAN,  "[INFO]    ") << text << std::endl;)
        break;

    case LogLevel::MESSAGE:
                      std::cerr      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::CYAN,  "[MESSAGE] ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::CYAN, text) << std::endl;
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " <<    Html::ColoredStr(Html   ::CYAN,  "[MESSAGE] ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::CYAN, text) << std::endl;)
        break;

    case LogLevel::LOG:
        RLSU_ON_DEBUG(std::cout      << BaseTabsStr() << text;)
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << text;)
        break;

    case LogLevel::DUMP:
        RLSU_ON_DEBUG(std::cout      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::SKYBLUE, "[DUMP]    ") << Console::ColoredFormatedCodePlace(code_place) << text;)
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " << Html   ::ColoredStr(Html   ::SKYBLUE, "[DUMP]    ") << Html   ::ColoredFormatedCodePlace(code_place) << text;)
        break;

    case LogLevel::WARNING:
                      std::cerr      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::YELLOW,  "[WARNING] ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::YELLOW, text) << std::endl;
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " <<    Html::ColoredStr(Html   ::YELLOW,  "[WARNING] ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::YELLOW, text) << std::endl;)
        break;

    case LogLevel::ERROR:
                      std::cerr      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::RED,     "[ERROR]   ") << Console::ColoredFormatedCodePlace(code_place) << Console::ColoredStr(Console::RED,   text) << std::endl;
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " <<    Html::ColoredStr(Html   ::RED,     "[ERROR]   ") << Html   ::ColoredFormatedCodePlace(code_place) << Html   ::ColoredStr(Html   ::RED,   text) << std::endl;)
        break;

    case LogLevel::VERIFY:
                      std::cerr      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::MAGENTA, "[VERIFY]  ") << Console::ColoredFormatedCodePlace(code_place) << (text.empty() ? "" : (Console::ColoredStr(Console::RED, "failed condition: ") + "'" + Console::ColoredStr(Console::MAGENTA, text) + "'"))  << std::endl;
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " << Html   ::ColoredStr(Html   ::MAGENTA, "[VERIFY]  ") << Html   ::ColoredFormatedCodePlace(code_place) << (text.empty() ? "" : (Html   ::ColoredStr(Html   ::RED, "failed condition: ") + "'" +    Html::ColoredStr(Html   ::MAGENTA, text) + "'"))  << std::endl;)
    break;

    case LogLevel::ASSERT:
                      std::cerr      << BaseTabsStr() << "[" + module_name + "] " << Console::ColoredStr(Console::MAGENTA, "[ASSERT]  ") << Console::ColoredFormatedCodePlace(code_place) << (text.empty() ? "" : (Console::ColoredStr(Console::RED, "failed condition: ") + "'" + Console::ColoredStr(Console::MAGENTA, text) + "'"))  << std::endl;
        RLSU_ON_DEBUG(GetLogfile_()  << BaseTabsStr() << "[" + module_name + "] " << Html   ::ColoredStr(Html   ::MAGENTA, "[ASSERT]  ") << Html   ::ColoredFormatedCodePlace(code_place) << (text.empty() ? "" : (Html   ::ColoredStr(Html   ::RED, "failed condition: ") + "'" +    Html::ColoredStr(Html   ::MAGENTA, text) + "'"))  << std::endl;)
        break;
    
    default:
        std::cout << "Unknown Log level!\n";
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