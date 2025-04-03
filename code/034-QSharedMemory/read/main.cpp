#include <QCoreApplication>
#include <QSharedMemory>
#include <QDebug>
#include <mutex>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QSharedMemory sm;
    sm.setKey("QSharedMemory1#");
    while (1) {
        if (!sm.attach()) {
            qDebug() << "attach SharedMemory error: " << sm.errorString();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }
        break;
    }
    while (1) {
        std::lock_guard<QSharedMemory> lock(sm);
        char* data = static_cast<char*>(sm.data());
        if (data[0] == 0) {
            qDebug() << "no data continue";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        qDebug() << "read: " << (data + 1);
        data[0] = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
    }
    sm.detach();

    return a.exec();
}
