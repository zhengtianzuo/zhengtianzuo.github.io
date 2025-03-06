#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp> 
#include <boost/interprocess/mapped_region.hpp> 

using namespace boost::interprocess;

int main() {
    // 1. 创建或打开共享内存 
    shared_memory_object shm(
        open_or_create, 
        "MySharedMemory", 
        read_write 
    );
    
    // 2. 设置内存大小（必须拥有写权限）
    shm.truncate(1024);   // 分配1024字节 
    
    // 3. 映射到当前进程 
    mapped_region region(shm, read_write);
    
    // 4. 写入数据 
    char* data = static_cast<char*>(region.get_address()); 
    std::strcpy(data, "Hello from Process A!");
    
    std::cout << "Data written to shared memory." << std::endl;
    // 注意：实际应用中需同步机制（如互斥锁）
    return 0;
}
