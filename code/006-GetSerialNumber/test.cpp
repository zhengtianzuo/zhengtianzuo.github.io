#include <iostream>
#include <string>
#include "SerialNumber.h"

int main() {
    std::cout << "CPU Serial: " << SerialNumber::GetCPUSerial() << std::endl;
    std::cout << "Memory Serial: " << SerialNumber::GetMemorySerial() << std::endl;
    std::cout << "GPU Serial: " << SerialNumber::GetGPUSerial() << std::endl;
    std::cout << "Disk Serial: " << SerialNumber::GetDiskSerial() << std::endl;
    return 0;
}
