#include "StdAfx.h"

#include "N3Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> CN3Log::s_Logger;

void CN3Log::Init(const std::string & loggerName) {
#ifdef _DEBUG
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

    FILE * fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(loggerName + ".log", true));
    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_Logger = std::make_shared<spdlog::logger>(loggerName, begin(logSinks), end(logSinks));
    s_Logger->set_level(spdlog::level::trace);
    s_Logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(s_Logger);
#endif
}

void CN3Log::Destroy() {
#ifdef _DEBUG
    spdlog::shutdown();
    FreeConsole();
#endif
}
