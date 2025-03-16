#include <iostream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define PORT 12345

// 自定义的重叠结构
typedef struct _PER_HANDLE_DATA {
    SOCKET socket;
    sockaddr_in clientAddr;
} PER_HANDLE_DATA, * PPER_HANDLE_DATA;

typedef struct _PER_IO_DATA {
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    char buffer[BUFFER_SIZE];
    int operationType;
#define OP_READ 1
#define OP_WRITE 2
} PER_IO_DATA, * PPER_IO_DATA;

// 工作线程函数
DWORD WINAPI WorkerThread(LPVOID lpParam) {
    HANDLE completionPort = (HANDLE)lpParam;
    DWORD bytesTransferred;
    PPER_HANDLE_DATA perHandleData;
    PPER_IO_DATA perIoData;

    while (true) {
        if (GetQueuedCompletionStatus(completionPort, &bytesTransferred, (PULONG_PTR)&perHandleData, (LPOVERLAPPED*)&perIoData, INFINITE) == 0) {
            std::cerr << "GetQueuedCompletionStatus failed: " << GetLastError() << std::endl;
            continue;
        }

        if (bytesTransferred == 0) {
            // 客户端关闭连接
            closesocket(perHandleData->socket);
            delete perHandleData;
            delete perIoData;
            continue;
        }

        if (perIoData->operationType == OP_READ) {
            perIoData->buffer[bytesTransferred] = '\0';
            std::cout << "Received from client: " << perIoData->buffer << std::endl;

            // 回显数据给客户端
            perIoData->operationType = OP_WRITE;
            perIoData->wsaBuf.len = bytesTransferred;
            DWORD flags = 0;
            WSASend(perHandleData->socket, &perIoData->wsaBuf, 1, NULL, 0, &perIoData->overlapped, NULL);
        }
        else if (perIoData->operationType == OP_WRITE) {
            // 继续读取数据
            perIoData->operationType = OP_READ;
            perIoData->wsaBuf.len = BUFFER_SIZE;
            perIoData->wsaBuf.buf = perIoData->buffer;
            DWORD flags = 0;
            WSARecv(perHandleData->socket, &perIoData->wsaBuf, 1, NULL, &flags, &perIoData->overlapped, NULL);
        }
    }

    return 0;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 创建监听套接字
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 监听连接
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 创建完成端口
    HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (completionPort == NULL) {
        std::cerr << "CreateIoCompletionPort failed: " << GetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 创建工作线程
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    for (DWORD i = 0; i < systemInfo.dwNumberOfProcessors * 2; ++i) {
        HANDLE threadHandle = CreateThread(NULL, 0, WorkerThread, completionPort, 0, NULL);
        if (threadHandle == NULL) {
            std::cerr << "CreateThread failed: " << GetLastError() << std::endl;
            closesocket(listenSocket);
            CloseHandle(completionPort);
            WSACleanup();
            return 1;
        }
        CloseHandle(threadHandle);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 为新连接创建 PER_HANDLE_DATA
        PPER_HANDLE_DATA perHandleData = new PER_HANDLE_DATA;
        perHandleData->socket = clientSocket;
        perHandleData->clientAddr = clientAddr;

        // 将新连接的套接字与完成端口关联
        if (CreateIoCompletionPort((HANDLE)clientSocket, completionPort, (ULONG_PTR)perHandleData, 0) == NULL) {
            std::cerr << "CreateIoCompletionPort for client socket failed: " << GetLastError() << std::endl;
            closesocket(clientSocket);
            delete perHandleData;
            continue;
        }

        // 为新连接创建 PER_IO_DATA 并开始读取数据
        PPER_IO_DATA perIoData = new PER_IO_DATA;
        ZeroMemory(&perIoData->overlapped, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = BUFFER_SIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        perIoData->operationType = OP_READ;
        DWORD flags = 0;
        WSARecv(clientSocket, &perIoData->wsaBuf, 1, NULL, &flags, &perIoData->overlapped, NULL);
    }

    // 清理资源
    closesocket(listenSocket);
    CloseHandle(completionPort);
    WSACleanup();

    return 0;
}