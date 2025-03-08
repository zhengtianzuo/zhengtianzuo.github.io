#pragma once

#ifdef _WIN32
#include <windows.h>
#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#endif

#ifdef __linux__
#include <fstream>
#include <unistd.h>
#include <limits>
#endif

#ifdef __APPLE__
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <unistd.h>
#include <libproc.h>
#include <mach/task_info.h>
#include <sys/sysctl.h>
#endif

class CpuMonitor {
public:
    float get_usage(int get_time_interval);
    float get_process_usage(int get_time_interval, int pid);
};
