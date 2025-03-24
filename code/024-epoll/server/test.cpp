#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <algorithm>
#include <fcntl.h>

#define MAX_EVENTS 1024
#define PORT 8888

class EpollServer {
public:
    EpollServer() : listen_fd(-1), epfd(-1) {}

    ~EpollServer() {
        if (listen_fd != -1) close(listen_fd);
        if (epfd != -1) close(epfd);
    }

    bool Init() {
        // 创建监听套接字
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd == -1) {
            std::cerr << "socket error" << std::endl;
            return false;
        }

        // 设置端口复用
        int opt = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // 绑定地址和端口
        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);

        if (bind(listen_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
            std::cerr << "bind error" << std::endl;
            return false;
        }

        // 监听
        if (listen(listen_fd, 5) == -1) {
            std::cerr << "listen error" << std::endl;
            return false;
        }

        // 创建epoll实例
        epfd = epoll_create1(0);
        if (epfd == -1) {
            std::cerr << "epoll_create1 error" << std::endl;
            return false;
        }

        // 添加监听套接字到epoll
        epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = listen_fd;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
            std::cerr << "epoll_ctl error" << std::endl;
            return false;
        }

        return true;
    }

    void Run() {
        std::cout << "Server running on port " << PORT << std::endl;

        while (true) {
            epoll_event events[MAX_EVENTS];
            int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
            if (nfds == -1) {
                std::cerr << "epoll_wait error" << std::endl;
                break;
            }

            for (int i = 0; i < nfds; ++i) {
                if (events[i].data.fd == listen_fd) {
                    // 处理新连接
                    sockaddr_in cli_addr;
                    socklen_t cli_len = sizeof(cli_addr);
                    int cli_fd = accept(listen_fd, (sockaddr*)&cli_addr, &cli_len);
                    if (cli_fd == -1) {
                        std::cerr << "accept error" << std::endl;
                        continue;
                    }

                    std::cout << "New connection from " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;

                    // 设置客户端套接字为非阻塞
                    int flags = fcntl(cli_fd, F_GETFL, 0);
                    fcntl(cli_fd, F_SETFL, flags | O_NONBLOCK);

                    // 添加客户端套接字到epoll
                    epoll_event event;
                    event.events = EPOLLIN | EPOLLET; // 边沿触发
                    event.data.fd = cli_fd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &event) == -1) {
                        std::cerr << "epoll_ctl error" << std::endl;
                        close(cli_fd);
                    }
                } else {
                    // 处理客户端数据
                    HandleClientData(events[i].data.fd);
                }
            }
        }
    }

private:
    void HandleClientData(int cli_fd) {
        char buffer[1024] = {0};
        ssize_t bytes_read = read(cli_fd, buffer, sizeof(buffer)-1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;

            // 回显数据
            write(cli_fd, buffer, bytes_read);
        } else if (bytes_read == 0) {
            // 客户端断开连接
            std::cout << "Client disconnected" << std::endl;
            epoll_ctl(epfd, EPOLL_CTL_DEL, cli_fd, nullptr);
            close(cli_fd);
        } else {
            // 读取错误
            std::cerr << "read error" << std::endl;
            epoll_ctl(epfd, EPOLL_CTL_DEL, cli_fd, nullptr);
            close(cli_fd);
        }
    }

    int listen_fd;
    int epfd;
};

int main() {
    EpollServer server;
    if (server.Init()) {
        server.Run();
    }
    return 0;
}
