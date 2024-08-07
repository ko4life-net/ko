#include "StdAfx.h"

#include "N3Log.h"

#if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> CN3Log::s_Logger;
#endif // #if defined(N3LOG_ENABLE_SPDLOG) && !defined(N3LOG_DISABLE_LOGS)

void CN3Log::Init(const std::string & szLoggerName) {
#if !defined(N3LOG_DISABLE_LOGS)
    AllocConsole();

    // Position console window at the top right corner
    HWND hConsole = GetConsoleWindow();
    RECT rcScreen{};
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);
    const int iScreenWidth = rcScreen.right - rcScreen.left;
    const int iScreenHeight = rcScreen.bottom - rcScreen.top;
    const int iTermWidth = 950;
    const int iTermHeight = 400;
    const int iPadding = 10;
    const int iX = iScreenWidth - iTermWidth - iPadding;
    const int iY = iPadding;
    SetWindowPos(hConsole, HWND_TOP, iX, iY, iTermWidth, iTermHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);

    // Redirect outputs
    FILE * fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
#if defined(N3LOG_ENABLE_SPDLOG)
    std::vector<spdlog::sink_ptr> vLogSinks;
    vLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    vLogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(szLoggerName + ".log", true));
    vLogSinks[0]->set_pattern("%^[%T] %n: %v%$");
    vLogSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_Logger = std::make_shared<spdlog::logger>(szLoggerName, begin(vLogSinks), end(vLogSinks));
    s_Logger->set_level(spdlog::level::trace);
    s_Logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(s_Logger);
#endif // #if defined(N3LOG_ENABLE_SPDLOG)
#endif // #if !defined(N3LOG_DISABLE_LOGS)
}

void CN3Log::Destroy() {
#if !defined(N3LOG_DISABLE_LOGS)
#if defined(N3LOG_ENABLE_SPDLOG)
    spdlog::shutdown();
#endif // #if defined(N3LOG_ENABLE_SPDLOG)
    FreeConsole();
#endif // #if !defined(N3LOG_DISABLE_LOGS)
}
