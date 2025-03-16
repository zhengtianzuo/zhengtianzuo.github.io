#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 12345

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // 连接到服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];

    while (true) {
        std::cout << "Enter message to send (or 'quit' to exit): ";
        std::cin.getline(sendBuffer, BUFFER_SIZE);

        if (strcmp(sendBuffer, "quit") == 0) {
            break;
        }

        // 发送数据
        int sendBytes = send(clientSocket, sendBuffer, strlen(sendBuffer), 0);
        if (sendBytes == SOCKET_ERROR) {
            std::cerr << "send failed: " << WSAGetLastError() << std::endl;
            break;
        }

        // 接收服务器回显数据
        int recvBytes = recv(clientSocket, recvBuffer, BUFFER_SIZE, 0);
        if (recvBytes == SOCKET_ERROR) {
            std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
        else if (recvBytes == 0) {
            // 服务器关闭连接
            std::cout << "Server closed the connection." << std::endl;
            break;
        }

        recvBuffer[recvBytes] = '\0';
        std::cout << "Received from server: " << recvBuffer << std::endl;
    }

    // 关闭套接字
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}