#include <iostream>
#include <iomanip>
#include "MemMonitor.h"

const int g_get_time_interval = 1;    // 获取内存使用率的时间间隔，单位秒

int main() {
    MemMonitor mem_monitor;
    while(1) {
        double usage = mem_monitor.get_usage();
        std::cout << "Mem Usage: " << std::fixed << std::setprecision(2) << usage << "%" << std::endl;

        double process_usage = mem_monitor.get_process_usage(6332);
        std::cout << "process CPU Usage: " << std::fixed << std::setprecision(2) << process_usage << "%" << std::endl;

        #ifdef _WIN32
            Sleep(g_get_time_interval*1000);
        #endif
        #ifdef __linux__
            sleep(g_get_time_interval);
        #endif
        #ifdef __APPLE__
            sleep(g_get_time_interval);
        #endif
    }
    return 0;
}
