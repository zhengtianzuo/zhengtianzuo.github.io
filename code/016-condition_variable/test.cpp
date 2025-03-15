#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadSafeQueue {
public:
    void Push(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mtx_);
        queue_.push(std::move(task));
        cv_.notify_one();  // 通知一个等待线程 
    }

    bool TryPop(std::function<void()>& task) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) return false;
        task = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void WaitPop(std::function<void()>& task) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{ return !queue_.empty() || stop_; });
        if (stop_) return;  // 处理终止信号 
        task = std::move(queue_.front());
        queue_.pop();
    }

    void Stop() {
        std::lock_guard<std::mutex> lock(mtx_);
        stop_ = true;
        cv_.notify_all();  // 唤醒所有线程退出 
    }

private:
    std::queue<std::function<void()>> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_ = false;
};

// 异步任务处理器 
class AsyncProcessor {
public:
    AsyncProcessor() : worker_([this]{ ProcessTasks(); }) {}

    ~AsyncProcessor() {
        queue_.Stop();
        worker_.join();
    }

    std::future<int> SubmitTask(int input) {
        auto promise = std::make_shared<std::promise<int>>();
        std::future<int> future = promise->get_future();
        queue_.Push([input, promise]{
            int result = input * 2; // 模拟耗时操作 
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            promise->set_value(result);
        });
        return future;
    }

private:
    using Task = std::function<void()>;
    ThreadSafeQueue queue_;
    std::thread worker_;

    void ProcessTasks() {
        while (true) {
            Task task;
            queue_.WaitPop(task);
            if (!task) break; // 收到终止信号 
            task(); // 执行实际任务 
        }
    }
};

// 使用示例 
int main() {
    AsyncProcessor processor;
    auto future = processor.SubmitTask(42);
    
    // 同步等待结果 
    std::cout << "start wait Result ..." << std::endl;
    std::cout << "Result: " << future.get()  << std::endl;
    return 0;
}