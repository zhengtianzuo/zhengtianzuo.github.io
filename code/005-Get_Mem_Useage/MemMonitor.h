#pragma once

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

#ifdef __linux__
#include <fstream>
#include <string>
#include <unordered_map>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <mach/mach_host.h>
#include <sys/sysctl.h>
#include <unistd.h>
#endif

class MemMonitor {
public:
    double get_usage();
    double get_process_usage(int pid);
};
