#include "MemMonitor.h"

#ifdef __APPLE__
    uint64_t get_total_memory() {
        int mib[2] = {CTL_HW, HW_MEMSIZE};
        uint64_t total_mem = 0;
        size_t len = sizeof(total_mem);
        sysctl(mib, 2, &total_mem, &len, NULL, 0);
        return total_mem;
    }
#endif

double MemMonitor::get_usage() {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength  = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.dwMemoryLoad;
#endif

#ifdef __linux__
    std::ifstream meminfo("/proc/meminfo");
    std::unordered_map<std::string, long> mem_data;
    std::string key;
    long value;

    while (meminfo >> key >> value) {
        meminfo.ignore(256,  '\n');
        mem_data[key] = value;
    }

    long total = mem_data["MemTotal:"];
    long available = mem_data["MemAvailable:"];
    long used = total - available;
    return (used * 100.0) / total;
#endif

#ifdef __APPLE__
    vm_size_t page_size;
    mach_port_t host_port = mach_host_self();
    mach_msg_type_number_t host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    vm_statistics_data_t vm_stat;

    host_page_size(host_port, &page_size);
    host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size);

    uint64_t total_mem = get_total_memory();
    uint64_t used_mem = (vm_stat.active_count  + vm_stat.wire_count)  * page_size;
    return (double)used_mem / total_mem * 100.0f;
#endif
    return 0;
}

double MemMonitor::get_process_usage(int pid) {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength  = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        SIZE_T usedMem = pmc.WorkingSetSize;
        SIZE_T totalMem = memInfo.ullTotalPhys;
        double ratio = (usedMem * 100.0) / totalMem;
        return ratio;
    }
    return 0;
#endif

#ifdef __linux__
    std::ifstream meminfo("/proc/meminfo");
    std::unordered_map<std::string, long> mem_data;
    std::string key;
    long value;

    while (meminfo >> key >> value) {
        meminfo.ignore(256,  '\n');
        mem_data[key] = value;
    }

    long total = mem_data["MemTotal:"];

    std::ifstream status("/proc/" + std::to_string(pid) + "/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.find("VmRSS:") == 0) {
            unsigned long rss = std::strtoul(line.substr(6).c_str(), nullptr, 10);
            status.close();
            return (double)rss*100/total;
        }
    }
    return 0;
#endif

#ifdef __APPLE__

#endif
    return 0;
}
