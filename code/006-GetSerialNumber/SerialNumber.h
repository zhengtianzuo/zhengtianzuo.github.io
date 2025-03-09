#pragma once
#include <iostream>

#if defined(_WIN32)
    #include <intrin.h>
    #include <comdef.h>
    #include <Wbemidl.h>
	#include <codecvt>
	
    #pragma comment(lib, "wbemuuid.lib")
#endif
#if defined(__linux__)
#include <cpuid.h>
#endif
#if defined(__APPLE__)
#include <cpuid.h>
#endif
class SerialNumber {
public:
    static std::string GetCPUSerial();
    static std::string GetMemorySerial();
    static std::string GetGPUSerial();
    static std::string GetDiskSerial();
};
