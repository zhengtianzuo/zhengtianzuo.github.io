---
icon: octicons/single-select-16
---

# 进程单例

以下是一篇跨平台进程单实例技术调研及实现方案，包含Windows/Linux/macOS三平台兼容的C++代码实现：

跨平台进程单实例技术实现
### 一、技术背景
* 进程单实例控制是保证关键服务唯一性的核心技术，常见于系统守护进程、桌面应用程序等场景。本文通过三种系统级机制实现跨平台支持：

* Windows：内核对象互斥体
* Linux/macOS：文件锁机制
* 通用方案：跨进程内存共享
### 二、实现方案
#### 方案一：跨平台文件锁（推荐）
~~~cpp
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
~~~

#### 方案二：共享内存检测
~~~cpp
// Windows实现
#ifdef _WIN32
#include <windows.h>

class WinSharedMem {
public:
    WinSharedMem(const char* name) {
        hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE, NULL, 
            PAGE_READWRITE, 0, 256, name);
        exists = (GetLastError() == ERROR_ALREADY_EXISTS);
    }
    // ... 其他成员函数
};
#endif

// Linux/macOS实现
#if defined(__linux__) || defined(__APPLE__)
#include <sys/mman.h>
#include <fcntl.h>

class UnixSharedMem {
public:
    UnixSharedMem(const char* name) {
        fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        exists = (errno == EEXIST);
    }
    // ... 其他成员函数
};
#endif
~~~
### 三、关键实现点
* Windows核心机制
使用CreateMutexA创建命名互斥体
通过GetLastError()检测ERROR_ALREADY_EXISTS
需显式释放互斥体资源
* Linux/macOS实现
采用flock()文件锁实现进程排他
锁文件存储在/tmp目录保证可写性
文件描述符自动回收机制
* 跨平台封装
使用预处理器指令区分平台
统一接口isAnotherRunning()
RAII模式管理资源生命周期
### 四、测试验证
#### Linux/macOS编译
~~~bash
g++ -std=c++11 singleton.cpp  -o singleton
~~~
#### Windows编译（VS开发者命令提示）
~~~bash
cl /EHsc singleton.cpp 
~~~
### 五、方案对比
|特性|	文件锁方案|	共享内存方案|
|-|-|-|
|跨平台性|	优秀|	需平台特定实现|
|可靠性	|高（内核级锁）	|中等|
|资源消耗	|低	|较高|
|实现复杂度|	简单	|复杂|

完整代码库可参考的实现模式，建议优先采用文件锁方案作为基础实现。实际部署时需注意权限管理（如Linux下的/tmp目录权限）和异常处理（如进程崩溃后的锁释放机制）。

## 完整代码
[Github](https://github.com/zhengtianzuo/zhengtianzuo.github.io/tree/master/code/015-SingleApplication)
