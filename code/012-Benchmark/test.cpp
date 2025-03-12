#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <execution>

// 测试不同排序算法的性能 
template <typename T>
static void BM_Sort(benchmark::State& state) {
    std::vector<T> data(state.range(0)); 
    std::generate(data.begin(),  data.end(),  []{ return rand() % 1000; });
    
    for (auto _ : state) {
        state.PauseTiming();
        auto temp = data; // 复制数据避免原地排序影响 
        state.ResumeTiming();
        
        std::sort(temp.begin(),  temp.end()); 
    }
    state.SetBytesProcessed(state.iterations()  * data.size()  * sizeof(T));
}
 
BENCHMARK_TEMPLATE(BM_Sort, int)->Range(1<<10, 1<<20)->Unit(benchmark::kMillisecond);
 
// 多线程版本 
static void BM_ParallelSort(benchmark::State& state) {
    std::vector<int> data(state.range(0)); 
    std::generate(data.begin(),  data.end(),  []{ return rand() % 1000; });
    
    for (auto _ : state) {
        auto temp = data;
        benchmark::DoNotOptimize(temp.data()); 
        std::sort(std::execution::par, temp.begin(),  temp.end()); 
    }
}
BENCHMARK(BM_ParallelSort)->Range(1<<20, 1<<24)->Threads(4)->UseRealTime();
 
BENCHMARK_MAIN();