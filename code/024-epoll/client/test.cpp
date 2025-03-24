#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#define PORT 8888
#define SERVER_IP "127.0.0.1"

class EpollClient {
public:
    EpollClient() : fd(-1) {}

    ~EpollClient() {
        if (fd != -1) close(fd);
    }

    bool Connect() {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            std::cerr << "socket error" << std::endl;
            return false;
        }

        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
            std::cerr << "invalid address/ address not supported" << std::endl;
            return false;
        }

        if (connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
            std::cerr << "connection failed" << std::endl;
            return false;
        }

        return true;
    }

    void SendData(const std::string& data) {
        if (fd == -1) return;

        write(fd, data.c_str(), data.size());
    }

    std::string ReceiveData() {
        if (fd == -1) return "";

        char buffer[1024] = {0};
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer)-1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            return std::string(buffer);
        }
        return "";
    }

private:
    int fd;
};

int main() {
    EpollClient client;
    if (client.Connect()) {
        std::cout << "Connected to server" << std::endl;

        // 发送数据
        client.SendData("Hello, server!");

        // 接收数据
        std::string response = client.ReceiveData();
        if (!response.empty()) {
            std::cout << "Received from server: " << response << std::endl;
        }

    } else {
        std::cerr << "Failed to connect to server" << std::endl;
    }
    return 0;
}
