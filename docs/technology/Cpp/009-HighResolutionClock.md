---
icon: material/clock
---

# 高精度计时器

以下是一个基于C++11标准库实现的跨平台高精度耗时计算方案，支持Windows/Linux/macOS系统，完整代码和实现原理如下：

### 一、实现原理
使用C++11引入的<chrono>库实现高精度计时，其优势在于：

* 跨平台性：标准库原生支持，无需平台特定API
* 高精度：high_resolution_clock可提供纳秒级精度
* 类型安全：强类型的时间单位和持续时间计算
* 易用性：无需手动转换时间单位，支持自动类型推导
### 二、完整代码实现
~~~cpp
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

class HighPrecisionTimer {
public:
    // 开始计时 
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    // 结束计时 
    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
    }
    
    // 获取耗时（默认毫秒）
    template<typename Duration = std::chrono::milliseconds>
    auto duration() const {
        return std::chrono::duration_cast<Duration>(end_time - start_time).count();
    }
    
    // 自动输出耗时结果 
    void print(const std::string& msg = "") const {
        auto ns = duration<std::chrono::nanoseconds>();
        std::cout << msg << "Cost: " << ns << " ns (" << ns/1000.0 << " μs, " << ns/1000000.0 << " ms)\n";
    }

private:
    std::chrono::high_resolution_clock::time_point start_time, end_time;
};

class AutoHighPrecisionTimer {
public:
    // 开始计时 
    AutoHighPrecisionTimer() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    // 获取耗时（默认毫秒）
    template<typename Duration = std::chrono::milliseconds>
    auto duration() const {
        return std::chrono::duration_cast<Duration>(end_time - start_time).count();
    }
    
    // 自动输出耗时结果 
    void print(const std::string& msg = "") {
        end_time = std::chrono::high_resolution_clock::now();
        auto ns = duration<std::chrono::nanoseconds>();
        std::cout << msg << "Cost: " << ns << " ns (" << ns/1000.0 << " μs, " << ns/1000000.0 << " ms)\n";
    }

private:
    std::chrono::high_resolution_clock::time_point start_time, end_time;
};

// 示例测试函数 
void test_function() {
    std::vector<int> data(1000000);
    std::generate(data.begin(),  data.end(),  [](){ return rand() % 1000; });
    std::sort(data.begin(),  data.end()); 
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main() {
    // 用例1：手动计时 
    HighPrecisionTimer timer;
    timer.start(); 
    test_function();
    timer.stop(); 
    timer.print("Manual  timing: ");
    
    // 用例2：自动作用域计时 
    {
        AutoHighPrecisionTimer auto_timer;
        test_function();
        auto_timer.print("Auto  scope timing: ");
    }
    
    return 0;
}

~~~
### 三、使用说明
* 基本计时
~~~cpp
HighPrecisionTimer t;
t.start(); 
// 被测代码 
t.stop(); 
std::cout << "耗时：" << t.duration()  << "ms\n";
~~~
* 指定时间单位
~~~cpp
auto microsec = t.duration<std::chrono::microseconds>(); 
auto nanosec = t.duration<std::chrono::nanoseconds>(); 
~~~
* 自动作用域计时
~~~cpp
{
    HighPrecisionTimer t;
    // 被测代码 
    t.print();  // 自动输出离开作用域时的耗时 
}
~~~
### 四、跨平台注意事项
* 编译要求：需开启C++11或更高标准
~~~bash
g++ -std=c++11 -O3 timer.cpp  -o timer 
~~~
* 精度说明 ：
Windows：通常提供100纳秒精度
Linux/macOS：通常提供1纳秒精度
实际精度取决于硬件支持

* 性能影响：
计时器本身开销约20-50纳秒
建议被测代码耗时>1微秒时使用
### 五、技术优势
* 类型安全的时间运算
~~~cpp
using namespace std::chrono_literals;
auto time1 = 100ms;  // 明确的时间单位 
auto time2 = 500us;  // 微秒字面量 
~~~
* 多时钟源支持
system_clock:  系统壁钟时间 
steady_clock:  单调递增时钟（推荐用于耗时计算）
high_resolution_clock: 最高精度时钟 

## 完整代码
[Github](https://github.com/zhengtianzuo/zhengtianzuo.github.io/tree/master/code/009-HighResolutionClock)
