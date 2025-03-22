
#include <iostream>
#include "cpplib.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#define LOGNAME "app"
#define LOG_TRACE(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_TRACE(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_DEBUG(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_DEBUG(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_INFO(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_INFO(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_WARN(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_WARN(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_ERROR(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_ERROR(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_CRITICAL(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_CRITICAL(spdlog::get(LOGNAME), __VA_ARGS__);}

void sub() {
	LOG_TRACE("这是应用程序的日志信息0");
	LOG_DEBUG("这是应用程序的日志信息1");
	LOG_INFO("这是应用程序的日志信息2");
	LOG_WARN("这是应用程序的日志信息3");
	LOG_ERROR("这是应用程序的日志信息4");
	LOG_CRITICAL("这是应用程序的日志信息5");
}

int main() {
	cpplib cpp;
	int result = cpp.add(123, 543);
	std::cout << "a: " << result << std::endl;

	spdlog::rotating_logger_mt(LOGNAME, "app.log", 104857610, 10);
	spdlog::get(LOGNAME)->set_pattern("%Y-%m-%d %H:%M:%S,%e %L %t %s:%# : %v");
	spdlog::get(LOGNAME)->set_level((spdlog::level::level_enum)SPDLOG_LEVEL_DEBUG);
	spdlog::get(LOGNAME)->flush_on((spdlog::level::level_enum)SPDLOG_LEVEL_TRACE);

	sub();
	return 0;
}
