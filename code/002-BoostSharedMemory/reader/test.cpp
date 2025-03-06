#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp> 
#include <boost/interprocess/mapped_region.hpp> 

using namespace boost::interprocess;

int main() {
    // 1. 打开已有共享内存 
    shared_memory_object shm(open_only, "MySharedMemory", read_only);
    
    // 2. 映射内存 
    mapped_region region(shm, read_only);
    
    // 3. 读取数据 
    char* data = static_cast<char*>(region.get_address()); 
    std::cout << "Received: " << data << std::endl;
    
    // 4. 清理（通常由最后一个进程执行）
    shared_memory_object::remove("MySharedMemory");
    return 0;
}
