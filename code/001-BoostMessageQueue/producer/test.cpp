#include <boost/interprocess/ipc/message_queue.hpp> 
#include <iostream>

using namespace boost::interprocess;

struct Message {
    int id;
    float value;
};

int main() {
    try {
        // 创建或打开消息队列
        message_queue mq(open_or_create, "demo_queue", 100, sizeof(Message));
        
        // 发送结构化消息
        for(int i=0; i<5; ++i) {
            Message msg = {i, i*1.5f};
            mq.send(&msg,  sizeof(msg), 0);
            std::cout << "Sent: ID=" << msg.id  << " Value=" << msg.value  << std::endl;
        }
    } 
    catch(interprocess_exception &ex) {
        std::cerr << "Error: " << ex.what()  << std::endl;
        return 1;
    }
    return 0;
}
