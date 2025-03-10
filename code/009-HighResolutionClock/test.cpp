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
