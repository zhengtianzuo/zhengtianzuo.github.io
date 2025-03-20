#include <iostream>
#include <chrono>
#include <atomic>

template<typename T, size_t Capacity>
class RingQueue {
private:
    T buffer[Capacity + 1]; // 多分配一个单元处理满队列判断 
    std::atomic<size_t> front{0};
    std::atomic<size_t> rear{0};
    
    // 内存屏障保证多核一致性
    static inline void memory_fence() { 
        std::atomic_thread_fence(std::memory_order_seq_cst);
    }
 
public:
    bool enqueue(const T& item) {
        size_t next_rear = (rear.load()  + 1) % (Capacity + 1);
        if(next_rear == front.load())  return false; // 队列满
        
        buffer[rear.load()] = item;
        rear.store(next_rear); 
        memory_fence();
        return true;
    }
 
    // 批量入队（返回成功插入数量）
    size_t batch_enqueue(const T* items, size_t count) {
        size_t current_rear = rear.load(std::memory_order_relaxed); 
        size_t free_space = (front.load(std::memory_order_acquire)  - current_rear - 1 + Capacity + 1) % (Capacity + 1);
        size_t actual_count = std::min(count, free_space);
 
        for(size_t i=0; i<actual_count; ++i) {
            size_t pos = (current_rear + i) % (Capacity + 1);
            buffer[pos] = items[i];
        }
        rear.store((current_rear  + actual_count) % (Capacity + 1), std::memory_order_release);
        return actual_count;
    }

    bool dequeue(T& item) {
        if(front.load()  == rear.load())  return false; // 队列空
        
        item = buffer[front.load()];
        front.store((front.load()  + 1) % (Capacity + 1));
        memory_fence();
        return true;
    }
 
    size_t size() const {
        return (rear.load()  - front.load()  + Capacity + 1) % (Capacity + 1);
    }
};

// 测试单线程批量入队吞吐量
void benchmark_batch_single_thread(RingQueue<int, 1024>& queue) {
    int batch_size = 100;
    int array[batch_size];
    for (size_t i = 0; i < batch_size; i++) {
        array[i] = i;
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<1e6; ++i) {
        int queuein = queue.batch_enqueue(array, batch_size); 
        if (queuein != batch_size) {
            std::cout << "batch_enqueue error: "<< queuein << std::endl;
            break;
        }
        int val;
        while(queue.dequeue(val)) {
        }
        
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "batch Throughput: " << 1e6*batch_size/(end-start).count() << " ops/ns" << std::endl;
}

// 测试单线程吞吐量
void benchmark_single_thread(RingQueue<int, 1024>& queue) {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<1e6; ++i){
        bool error = queue.enqueue(i);
        if (error != true) {
            std::cout << "enqueue error" << std::endl;
            break;
        }
        int val;
        error = queue.dequeue(val);
        if (error != true) {
            std::cout << "dequeue error" << std::endl;
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Throughput: " << 1e6/(end-start).count() << " ops/ns" << std::endl;
}

int main() {
    RingQueue<int, 1024> queue;
    benchmark_single_thread(queue);
    benchmark_batch_single_thread(queue);
    return 0;
}
