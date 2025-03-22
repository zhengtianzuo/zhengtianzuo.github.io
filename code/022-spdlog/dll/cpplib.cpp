#include "cpplib.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#define LOGNAME "lib"
#define LOG_TRACE(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_TRACE(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_DEBUG(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_DEBUG(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_INFO(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_INFO(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_WARN(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_WARN(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_ERROR(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_ERROR(spdlog::get(LOGNAME), __VA_ARGS__);}
#define LOG_CRITICAL(...) if (spdlog::get(LOGNAME)) {SPDLOG_LOGGER_CRITICAL(spdlog::get(LOGNAME), __VA_ARGS__);}

cpplib::cpplib()
{
    spdlog::rotating_logger_mt(LOGNAME, "lib.log", 104857610, 10);
    spdlog::get(LOGNAME)->set_pattern("%Y-%m-%d %H:%M:%S,%e %L %t %s:%# : %v");
    spdlog::get(LOGNAME)->set_level((spdlog::level::level_enum)SPDLOG_LEVEL_INFO);
    spdlog::get(LOGNAME)->flush_on((spdlog::level::level_enum)SPDLOG_LEVEL_TRACE);
}

cpplib::~cpplib()
{
}

int cpplib::add(int a, int b) {
	LOG_TRACE("这是动态库的日志信息0");
	LOG_DEBUG("这是动态库的日志信息1");
	LOG_INFO("这是动态库的日志信息2");
	LOG_WARN("这是动态库的日志信息3");
	LOG_ERROR("这是动态库的日志信息4");
	LOG_CRITICAL("这是动态库的日志信息5");
    return a+b;
}

int cpplib_add(int a, int b) {
    return cpplib::add(a, b);
}

