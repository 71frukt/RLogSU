#pragma once

// #include "RLogSU/logger.hpp"

namespace RLSU {

// // class Message : public std::string
// // {
// // public:
// //     using std::string::string;
// //     Message(const std::string& str) : std::string(str) {}
// //     Message(const char* str)        : std::string(str) {}
// // };

// // using Format = Message;

// template <typename T>
// class RetErrorCode
// {
// public:
//     RetErrorCode(const T& value) : value_(value) {}
//     RetErrorCode(T&& value) : value_(std::move(value)) {}
    
//     // Оператор преобразования к базовому типу
//     operator T() const { return value_; }

//     T get() const { return value_; }
// private:
//     T value_;
// };


// template <typename T>
// class FuncErrorCode
// {
// public:
//     FuncErrorCode(const T& value) : value_(value) {}
//     FuncErrorCode(T&& value) : value_(std::move(value)) {}
    
//     // Оператор преобразования к базовому типу
//     operator T() const { return value_; }

//     T get() const { return value_; }
// private:
//     T value_;
// };


// namespace VerificatorFunctions
// {
//     inline ExitCode get_func_exit_val(std::string& res_message)
//     {
//         res_message = "";
//         return ExitCode::FAIL;
//     }
    
//     template<typename T>
//     T get_func_exit_val(std::string& res_message, const RetErrorCode<T>& error_code)
//     {
//         res_message = "";
//         return error_code.get();
//     }

//     inline ExitCode get_func_exit_val(std::string& res_message, const std::string& message)
//     {
//         // std::vformat(std::string_view(format), std::make_format_args(args...));
//         // RLSU_LOG("message: " + message + "\n\n");

//         res_message = "message: " + message + "\n\n";

//         return ExitCode::FAIL;
//     }

//     template<typename... Args>
//     inline ExitCode get_func_exit_val(std::string& res_message, const std::string& format, Args... args)
//     {
//         // RLSU_LOG("message: " + fromat + "\n\n", args...);
//         res_message = "message: " + std::vformat(std::string_view(format), std::make_format_args(args...)) + "\n\n";
//         return ExitCode::FAIL;
//     }

//     template<typename T>
//     T get_func_exit_val(std::string& res_message, const RetErrorCode<T>& exit_value, const std::string& message)
//     {
//         res_message = "message: " + message + "\n\n";
//         return exit_value;
//     }

//     template<typename T,  typename... Args>
//     T get_func_exit_val(std::string& res_message, const RetErrorCode<T>&exit_value, const std::string& format, Args... args)
//     {
//         // RLSU_LOG("message: " + fromat + "\n\n", args...);
//         res_message = "message: " + std::vformat(std::string_view(format), std::make_format_args(args...)) + "\n\n";
//         return exit_value;
//     }
// }

// namespace ErrorHandleFunctions
// {
//     inline ExitCode get_ret_exit_val()
//     {
//         return ExitCode::FAIL;
//     }

//     template<typename T>
//     inline ExitCode get_ret_exit_val(const RetErrorCode<T>& exit_value)
//     {
//         return ExitCode::FAIL;
//     }
// }

// // ... = exit_val returning if func fails
// #define ERROR_HANDLE(CallFunc, ...)                                                 \
// do {                                                                                \
//     auto exit_code = CallFunc;                                                      \
//                                                                                     \
//     if (exit_code == RLSU::VerificatorFunctions::get_func_exit_val(__VA_ARGS__))    \
//     {                                                                               \
//         RLSU_ERROR("Call failed: '{}'", #CallFunc);                                 \
//     }                                                                               \
// } while(0)


// ... = exit_val returning if func fails
#define ERROR_HANDLE(_CallFunc, ...)                                            \
do {                                                                            \
    try {                                                                       \
        _CallFunc;                                                              \
    }                                                                           \
    catch (const std::runtime_error& _e) {                                      \
        RLSU_ERROR("Call failed: '{}'. what(): {}", #_CallFunc, _e.what());     \
        throw;                                                                  \
    }                                                                           \
} while(0)

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