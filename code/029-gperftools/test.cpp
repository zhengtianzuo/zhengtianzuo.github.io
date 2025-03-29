
#include <gperftools/profiler.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stl_numeric.h>

// 一个存在性能问题的函数
void slow_function() {
    for (int i = 0; i < 10000; ++i) {
        std::vector<int> v;
        for (int j = 0; j < 1000; ++j) {
            v.push_back(j);
        }
        std::reverse(v.begin(), v.end());
    }
}

// 一个较快的函数作为对比
void fast_function() {
    for (int i = 0; i < 1000; ++i) {
        std::vector<int> v(1000);
        std::iota(v.begin(), v.end(), 0);
    }
}

int main() {
    ProfilerStart("output.prof");  // 启动性能分析
    
    std::cout << "Starting performance test...\n";
    
    slow_function();
    fast_function();
    
    ProfilerStop();  // 停止性能分析
    std::cout << "Performance test completed.\n";
    return 0;
}