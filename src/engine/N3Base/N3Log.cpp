#include "StdAfx.h"

#include "N3Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> CN3Log::s_Logger;

void CN3Log::Init(const std::string & loggerName)
{
#ifdef _DEBUG
	AllocConsole();
	HWND hConsole = GetConsoleWindow();
	int xpos = 1024;
	int ypos = 0;
	SetWindowPos(hConsole, 0, xpos, ypos, 0, 0, SWP_NOSIZE);

	FILE* fDummy;
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

void CN3Log::Destroy()
{
#ifdef _DEBUG
	spdlog::shutdown();
	FreeConsole();
#endif
}
