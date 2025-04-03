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
    sm.create(1024*1024);

    int nIndex = 0;
    while (1) {
        std::lock_guard<QSharedMemory> lock(sm);
        char* data = static_cast<char*>(sm.data());
        QString strInfo = "QSharedMemory1# with index: " + QString::number(nIndex);
        strcpy(data + 1, strInfo.toStdString().c_str());
        data[0] = 1;
        qDebug() << "write done: " << nIndex;
        nIndex++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
    }

    return a.exec();
}
