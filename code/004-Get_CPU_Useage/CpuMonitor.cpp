#include "CpuMonitor.h"

#ifdef __linux__
struct CpuTime {
    unsigned long user, nice, system, idle, iowait, irq, softirq;
};

CpuTime readCpuStats() {
    std::ifstream file("/proc/stat");
    std::string cpu_label;
    CpuTime time;
    file >> cpu_label >> time.user  >> time.nice  >> time.system  >> time.idle >> time.iowait  >> time.irq  >> time.softirq; 
    return time;
}

float calculate(const CpuTime& prev, const CpuTime& curr) {
    const unsigned long prev_total = prev.user  + prev.nice  + prev.system  + prev.idle + prev.iowait  + prev.irq  + prev.softirq; 
    const unsigned long curr_total = curr.user  + curr.nice  + curr.system  + curr.idle + curr.iowait  + curr.irq  + curr.softirq; 
    const unsigned long total_diff = curr_total - prev_total;

    const unsigned long idle_diff = curr.idle  - prev.idle; 
    return (total_diff - idle_diff) * 100.0 / total_diff;
}
#endif

#ifdef __APPLE__
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>

struct CpuUsage {
    uint64_t userTime;
    uint64_t systemTime;
};

CpuUsage getProcessCpuTime(pid_t pid) {
    struct proc_taskinfo taskInfo;
    memset(&taskInfo, 0, sizeof(taskInfo));
    if (proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &taskInfo, sizeof(taskInfo)) != sizeof(taskInfo)) {
        return {0, 0};
    }

    return {
        taskInfo.pti_total_user,
        taskInfo.pti_total_system
    };
}

uint64_t getSystemCpuTime() {
    host_cpu_load_info_data_t cpuInfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuInfo, &count) != KERN_SUCCESS) {
        return 0;
    }

    uint64_t totalSystemTime = 0;
    for (int i = 0; i < count; ++i) {
        totalSystemTime += cpuInfo.cpu_ticks[i];
    }

    return totalSystemTime;
}

float calculateCpuUsage(pid_t pid, uint64_t previousProcessTime, uint64_t previousSystemTime) {
    CpuUsage currentProcessTime = getProcessCpuTime(pid);
    uint64_t currentSystemTime = getSystemCpuTime();

    uint64_t processTimeDiff = currentProcessTime.userTime + currentProcessTime.systemTime - previousProcessTime;
    uint64_t systemTimeDiff = currentSystemTime - previousSystemTime;

    if (systemTimeDiff == 0) {
        return 0.0;
    }

    return (static_cast<float>(processTimeDiff) / static_cast<float>(systemTimeDiff)) * 100.0;
}
#endif

float CpuMonitor::get_usage(int get_time_interval) {
#ifdef _WIN32
    PDH_HQUERY query;
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        return 0;
    }

    PDH_HCOUNTER counter;
    status = PdhAddCounter(query, "\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        PdhCloseQuery(query);
        return 0;
    }

    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        PdhCloseQuery(query);
        return 0;
    }

    Sleep(get_time_interval*1000);

    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        PdhCloseQuery(query);
        return 0;
    }

    PDH_FMT_COUNTERVALUE value;
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &value);
    if (status != ERROR_SUCCESS) {
        PdhCloseQuery(query);
        return 0;
    }

    PdhCloseQuery(query);
    return value.doubleValue;
#endif

#ifdef __linux__
    CpuTime prev = readCpuStats();
    sleep(get_time_interval);
    CpuTime curr = readCpuStats();
    return calculate(prev, curr);
#endif

#ifdef __APPLE__
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    kern_return_t kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count);
    if (kr != KERN_SUCCESS) return 0;

    uint64_t total_user_prev = cpuinfo.cpu_ticks[CPU_STATE_USER]; 
    uint64_t total_sys_prev = cpuinfo.cpu_ticks[CPU_STATE_SYSTEM]; 
    uint64_t total_idle_prev = cpuinfo.cpu_ticks[CPU_STATE_IDLE]; 
    uint64_t total_used_prev = total_user_prev + total_sys_prev;

    sleep(get_time_interval);

    kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count);
    if (kr != KERN_SUCCESS) return 0;

    uint64_t total_user = cpuinfo.cpu_ticks[CPU_STATE_USER]; 
    uint64_t total_sys = cpuinfo.cpu_ticks[CPU_STATE_SYSTEM]; 
    uint64_t total_idle = cpuinfo.cpu_ticks[CPU_STATE_IDLE]; 
    uint64_t total_used = total_user + total_sys;

    uint64_t used_diff = total_used - total_used_prev;
    uint64_t idle_diff = total_idle - total_idle_prev;
    uint64_t total_diff = used_diff + idle_diff;

    return (total_diff == 0) ? 0 : (used_diff * 100.0) / total_diff;
#endif
    return 0;
}

float CpuMonitor::get_process_usage(int get_time_interval, int pid) {
#ifdef _WIN32
    if (pid == -1) pid = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess == nullptr) return 0;
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime)) return 0;

    ULONGLONG lastKernel = *(ULONGLONG*)&kernelTime;
    ULONGLONG lastUser = *(ULONGLONG*)&userTime;
    ULONGLONG lastTick = GetTickCount64();

    Sleep(get_time_interval*1000);

    if (!GetProcessTimes(hProcess, &createTime, &exitTime, &kernelTime, &userTime)) return 0;
    ULONGLONG currentKernel = *(ULONGLONG*)&kernelTime;
    ULONGLONG currentUser = *(ULONGLONG*)&userTime;

    ULONGLONG delta = (currentKernel + currentUser) - (lastKernel + lastUser);
    ULONGLONG timePassed = GetTickCount64() - lastTick;

    if (timePassed == 0) return 0.0f;
    return (delta / 10000.0f) / timePassed * 100.0f;
#endif

#ifdef __linux__
    if (pid == -1) pid = gettid();
    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    if (!statFile.is_open()) return 0;
    unsigned long long utime, stime, cutime, cstime, starttime;
    for (int i = 0; i < 13; ++i) {
        statFile.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    }
    statFile >> utime >> stime >> cutime >> cstime >> starttime;
    unsigned long long totalCpuTime = utime + stime + cutime + cstime;

    CpuTime curr = readCpuStats();
    unsigned long long systemTotalCpuTime = curr.user  + curr.nice  + curr.system + curr.iowait  + curr.irq  + curr.softirq;
    return (totalCpuTime * 100.0) / systemTotalCpuTime;
#endif

#ifdef __APPLE__
	
	pid_t p_pid = pid;
	CpuUsage initialProcessTime = getProcessCpuTime(p_pid);
    uint64_t initialSystemTime = getSystemCpuTime();

    sleep(get_time_interval);

    float cpuUsage = calculateCpuUsage(p_pid, initialProcessTime.userTime + initialProcessTime.systemTime, initialSystemTime);
	return cpuUsage;
#endif
}
