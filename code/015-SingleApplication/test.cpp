#include <iostream>
#include <string>
#include <cstdlib>
 
#if defined(_WIN32)
#include <Windows.h>
#else 
#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#endif 
 
class SingleInstance {
public:
    SingleInstance(const std::string& lockName) {
#if defined(_WIN32)
        // Windows命名互斥体
        hMutex = CreateMutexA(NULL, TRUE, lockName.c_str());
        isRunning = (GetLastError() == ERROR_ALREADY_EXISTS);
#else 
        // Unix系文件锁
        std::string lockPath = "/tmp/" + lockName + ".lock";
        fd = open(lockPath.c_str(), O_CREAT | O_RDWR, 0666);
        isRunning = (flock(fd, LOCK_EX | LOCK_NB) != 0);
#endif 
    }
 
    ~SingleInstance() {
#if defined(_WIN32)
        if(hMutex) {
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
        }
#else 
        if(fd >= 0) {
            flock(fd, LOCK_UN);
            close(fd);
        }
#endif 
    }
 
    bool isAnotherRunning() const { return isRunning; }
 
private:
#if defined(_WIN32)
    HANDLE hMutex = NULL;
#else 
    int fd = -1;
#endif 
    bool isRunning = false;
};
 
// 使用示例 
int main() {
    SingleInstance si("my_app");
    if(si.isAnotherRunning())  {
        std::cerr << "Application already running!" << std::endl;
        return EXIT_FAILURE;
    }
 
    // 主程序逻辑 
    std::cout << "Application started successfully" << std::endl;
    std::cin.get(); 
    return EXIT_SUCCESS;
}
