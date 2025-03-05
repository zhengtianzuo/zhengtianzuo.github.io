#include <boost/interprocess/ipc/message_queue.hpp> 
#include <iostream>

using namespace boost::interprocess;

struct Message {
    int id;
    float value;
};

int main() {
    try {
        // 打开现有队列
        message_queue mq(open_only, "demo_queue");
        
        Message msg;
        size_t recvd_size;
        unsigned int priority;
        
        // 持续接收消息
        while(true) {
            mq.receive(&msg,  sizeof(msg), recvd_size, priority);
            if(recvd_size == sizeof(msg)) {
                std::cout << "Received: ID=" << msg.id << " Value=" << msg.value  << std::endl;
            }
        }
    }
    catch(interprocess_exception &ex) {
        message_queue::remove("demo_queue");
        std::cerr << ex.what()  << std::endl;
        return 1;
    }
    return 0;
}
