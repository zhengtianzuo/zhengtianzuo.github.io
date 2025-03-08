#include <iostream>
#include <iomanip>
#include "CpuMonitor.h"

int main() {
    CpuMonitor cpu_monitor;
    while(1) {
        double usage = cpu_monitor.get_usage(1);
        std::cout << "CPU Usage: " << std::fixed << std::setprecision(2) << usage << "%" << std::endl;

        double process_usage = cpu_monitor.get_process_usage(1, 8423);
        std::cout << "process CPU Usage: " << std::fixed << std::setprecision(2) << process_usage << "%" << std::endl;
    }
    return 0;
}
