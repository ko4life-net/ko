// This class supports flexible logging configurations based on preprocessor directives. You can control logging behavior with the following options:
//
// N3LOG_DISABLE_LOGS: Disables all logging functionality, including console window allocation.
// N3LOG_ENABLE_SPDLOG: Activates spdlog for logging, assuming spdlog is included in your project's dependencies.
//
// If neither directive is defined, a basic iostream-based logging implementation will be used.
// While you can interact with the CN3Log class directly, it is recommended to use the provided macros.
// This allows you to define N3LOG_DISABLE_LOGS in release mode, ensuring that logging operations are effectively disabled without altering your code.
#pragma once

#include <string>

// Automatically disable logs in release mode unless logging is explicitly enabled.
// To enable logging in release mode, explicitly define N3LOG_ENABLE_LOGS.
#if !defined(_DEBUG) && !defined(N3LOG_ENABLE_LOGS)
#define N3LOG_DISABLE_LOGS
#endif // #if !defined(_DEBUG) && !defined(N3LOG_ENABLE_LOGS)

#if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)
// TODO: Note that SPDLOG_USE_STD_FORMAT can be removed once the project is configured via cmake
// As it adds this preprocessor into the build configuration via target interface automatically.
#define SPDLOG_USE_STD_FORMAT

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#endif // #if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)

class CN3Log {
  public:
    static void Init(const std::string & szLoggerName);
    static void Destroy();

#if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)
    static std::shared_ptr<spdlog::logger> & GetLogger() { return s_Logger; }

  private:
    static std::shared_ptr<spdlog::logger> s_Logger;
#endif // #if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)
};

#if defined(N3LOG_DISABLE_LOGS)

#define N3_LOGGER_INIT(name) (void)0
#define N3_LOGGER_DESTROY()  (void)0
#define N3_TRACE(...)        (void)0
#define N3_DEBUG(...)        (void)0
#define N3_INFO(...)         (void)0
#define N3_WARN(...)         (void)0
#define N3_ERROR(...)        (void)0
#define N3_FATAL(...)        (void)0
#define N3_ASSERT(x, msg)    (void)0

#elif defined(N3LOG_ENABLE_SPDLOG)

#define N3_LOGGER_INIT(name) CN3Log::Init(name)
#define N3_LOGGER_DESTROY()  CN3Log::Destroy()
#define N3_TRACE(...)        CN3Log::GetLogger()->trace(__VA_ARGS__)
#define N3_DEBUG(...)        CN3Log::GetLogger()->debug(__VA_ARGS__)
#define N3_INFO(...)         CN3Log::GetLogger()->info(__VA_ARGS__)
#define N3_WARN(...)         CN3Log::GetLogger()->warn(__VA_ARGS__)
#define N3_ERROR(...)        CN3Log::GetLogger()->error(__VA_ARGS__)
#define N3_FATAL(...)        CN3Log::GetLogger()->critical(__VA_ARGS__)
#define N3_ASSERT(x, msg)                                                                                              \
    if ((x)) {                                                                                                         \
    } else {                                                                                                           \
        N3_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__);                      \
        __debugbreak();                                                                                                \
    }

#else

#include <format>
#include <iostream>

#define N3_LOGGER_INIT(name) CN3Log::Init(name)
#define N3_LOGGER_DESTROY()  CN3Log::Destroy()

#define N3_LOG(level, ...)                                                                                             \
    do {                                                                                                               \
        std::string msg = std::format(__VA_ARGS__);                                                                    \
        std::cout << "[" << level << "] " << msg << std::endl;                                                         \
    } while (false)

#define N3_TRACE(...) N3_LOG("TRACE", __VA_ARGS__)
#define N3_DEBUG(...) N3_LOG("DEBUG", __VA_ARGS__)
#define N3_INFO(...)  N3_LOG("INFO", __VA_ARGS__)
#define N3_WARN(...)  N3_LOG("WARN", __VA_ARGS__)
#define N3_ERROR(...) N3_LOG("ERROR", __VA_ARGS__)
#define N3_FATAL(...) N3_LOG("FATAL", __VA_ARGS__)
#define N3_ASSERT(x, msg)                                                                                              \
    if (!(x)) {                                                                                                        \
        N3_FATAL("ASSERT - {} \n\t{} \n\tin file: {} \n\ton line: {}", #x, msg, __FILE__, __LINE__);                   \
        __debugbreak();                                                                                                \
    }

#endif // #if defined(N3LOG_DISABLE_LOGS)
