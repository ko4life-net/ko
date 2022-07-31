#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class CN3Log
{
public:
	static void Init(const std::string & loggerName);
	static void Destroy();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};


#ifdef _DEBUG
#define N3_TRACE(...)	CN3Log::GetLogger()->trace(__VA_ARGS__)
#define N3_DEBUG(...)	CN3Log::GetLogger()->debug(__VA_ARGS__)
#define N3_INFO(...)	CN3Log::GetLogger()->info(__VA_ARGS__)
#define N3_WARN(...)	CN3Log::GetLogger()->warn(__VA_ARGS__)
#define N3_ERROR(...)	CN3Log::GetLogger()->error(__VA_ARGS__)
#define N3_FATAL(...)	CN3Log::GetLogger()->critical(__VA_ARGS__)
#define N3_ASSERT(x, msg) if ((x)) {} else { N3_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); __debugbreak(); }
#else
#define N3_TRACE(...)		(void)0
#define N3_DEBUG(...)		(void)0
#define N3_INFO(...)		(void)0
#define N3_WARN(...)		(void)0
#define N3_ERROR(...)		(void)0
#define N3_FATAL(...)		(void)0
#define N3_ASSERT(x, msg)	(void)0
#endif
