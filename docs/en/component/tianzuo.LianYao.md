# tianzuo.LianYao

![](../img/tianzuo.LianYao.jpg)

### tianzuo.LianYao

![](https://img.shields.io/badge/release-1.0.0.0-blue.svg)
![](https://img.shields.io/badge/date-24.1.1-orange.svg)

||||
|--|--|--|
|operating system|![](https://img.shields.io/badge/os-windows_7+-blue.svg) ![](https://img.shields.io/badge/os-macos_10.14+-lightgrey.svg) ![](https://img.shields.io/badge/os-ubuntu_20.04+-orange.svg)||
|compiler|![](https://img.shields.io/badge/c++-11-blue.svg) ![](https://img.shields.io/badge/msvc-14.0-blue.svg) ![](https://img.shields.io/badge/msvc-14.1-blue.svg) ![](https://img.shields.io/badge/msvc-14.2-blue.svg) ![](https://img.shields.io/badge/msvc-14.3-blue.svg) ![](https://img.shields.io/badge/ndk-21.3-green.svg) ![](https://img.shields.io/badge/llvm-10.0-lightgrey.svg) ![](https://img.shields.io/badge/gcc-9.4-orange.svg)||
|programming language|![](../img/C.png) ![](../img/C__.png) ||

# introduction

![](../img/alchemy_tianzuo.LianYao.jpg)

**tianzuo.LianYao** A simple memory pool that initializes some memory for recycling, dynamically expands when insufficient, and has memory leak detection function.

**legend:**
It represents fusion and evolution. The demon pot can contain all things, and refine the demon beasts and spirit beasts into the vitality of the pot. This power symbolizes integration and evolution, and implies the tolerance and harmony of Taoist culture.

# motivation
A memory pool is designed to use memory in cycles, improve memory usage efficiency, and provide memory leak detection function.

![](../img/tianzuo.LianYaoInfo.png)

# features

- 🧩 simple integration
- 📝 optionally generate memory leak log
- 🪐 log in multiple languages (simplified chinese / english)

# screenshot

**locate memory leak lines of code through log file analysis:**

![](../img/tianzuo.LianYao1_en.png)

# example

```cpp

#include <iostream>
#include "../../include/tianzuo.LianYaoInterface.h"

void* operator new(size_t size) {
    return tianzuo_LianYaoInt::mem_get(size);
}
void* operator new[](size_t size) {
    return operator new(size);
}

void operator delete(void* ptr) noexcept {
    tianzuo_LianYaoInt::mem_del(ptr);
}
void operator delete[](void* ptr) noexcept {
    operator delete(ptr);
}

class cpp {
 public:
    cpp() {}
    ~cpp() {}
 private:
     int m_a;
     long m_l;
     float m_f;
};

int main(int argc, char* argv[]) {
    std::cout << "start tianzuo.LianYaoTest ...\n";
#ifdef WIN32
    tianzuo_LianYaoInt::initialize(3, "memory_leaks.log", nullptr, 0);
#else
    tianzuo_LianYaoInt::initialize(3, "memory_leaks.log", "./tianzuo.LianYaoTest", 0);
#endif

    int* new_int = new int(10); // leak 4
    int* new_int2 = new int(10);
    *new_int2 = 12;
    
    for (size_t i = 0; i < 10; i++) {
        int* new_int3 = new int(10);
        *new_int3 = 12;
        delete new_int3;
    }
   
    char* new_buf = new char[1555]; // leak 1555
    char* new_buf2 = new char[123];

    cpp* new_class = new cpp(); // leak 12
    cpp* new_class2 = new cpp();
    
    class1* cls1 = new class1();
    delete cls1;

    delete new_class2;
    delete new_int2;
    delete[] new_buf2;

    tianzuo_LianYaoInt::terminate();

    return 0;
}

```

# download

|||||
|--|--|--|--|
|[![download](../img/com_btnGitHub.svg)](https://github.com/zhengtianzuo/tianzuo.LianYao/releases)|[![download](../img/com_btnGitee.svg)](https://gitee.com/zhengtianzuo/tianzuo.LianYao/releases)|[![download](../img/down_baidu.svg)](https://pan.baidu.com/s/1Lv9dOBpV4-eXpi9yl0BSUg?pwd=1234)|[![download](../img/down_weiyun.svg)](https://share.weiyun.com/J7ZYxKL7)|
