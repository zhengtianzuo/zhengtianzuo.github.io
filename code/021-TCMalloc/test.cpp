#include <benchmark/benchmark.h>
#include "tcmalloc.h"
#include <vector>
#include <cstring>

static void BM_Malloc(benchmark::State& state) {
    size_t size = state.range(0);
    for (auto _ : state) {
        void* ptr = malloc(size);
        if (ptr == nullptr) {
            state.SkipWithError("Failed to allocate memory");
        }
        // 初始化分配的内存，避免编译器优化掉分配操作
        memset(ptr, 0, size);
        free(ptr);
    }
}
BENCHMARK(BM_Malloc)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(128)->Arg(1024)->Arg(4096)->Arg(16384)->Arg(65536)->Arg(131072)->Arg(1048576);

BENCHMARK_MAIN();