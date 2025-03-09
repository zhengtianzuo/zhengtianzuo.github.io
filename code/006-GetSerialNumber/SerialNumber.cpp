#include "SerialNumber.h"
#define BUFFERLEN 1024

#if defined(_WIN32)
std::string wstring2string(const std::wstring& wstr) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}
#endif

std::string& string_trim(std::string &s) {
    if (s.empty()) return s;
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

std::string removeSubstring(std::string str, const std::string& substr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        str.erase(pos, substr.length());
    }
    return str;
}

std::string SerialNumber::GetCPUSerial() {
#if defined(_WIN32)
    DWORD cpuInfo[2] = { 0 };
    __cpuid((int*)cpuInfo, 1);
    char cpuSerialNumber[BUFFERLEN];
    std::sprintf(cpuSerialNumber, "%08x-%08x", cpuInfo[0], cpuInfo[1]);
    return std::string(cpuSerialNumber);
#elif defined(__linux__)
    char buffer[BUFFERLEN];
    FILE* pipe = popen("dmidecode -t processor | grep 'ID'", "r");
    fgets(buffer, BUFFERLEN, pipe);
    pclose(pipe);
    return std::string(buffer).substr(5);
#elif defined(__APPLE__)
    std::string serial = "";
    FILE *fp = popen("sysctl -n machdep.cpu.brand_string", "r");
    if (fp) {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), fp)) {
            serial += buffer;
            break;
        }
        pclose(fp);
    }
    return serial;
#endif 
}

std::string SerialNumber::GetMemorySerial() {
#if defined(_WIN32)
    HRESULT hRes = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hRes)) {
        CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    }
    if (FAILED(hRes)) return "";
    hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    if (FAILED(hRes)) {
        CoUninitialize();
        return "";
    }

    IWbemLocator* pLoc = NULL;
    hRes = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,(LPVOID*)&pLoc);
    if (FAILED(hRes)) {
        CoUninitialize();
        return "";
    }

    IWbemServices* pSvc = NULL;
    hRes = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hRes)) {
        pLoc->Release();
        CoUninitialize();
        return "";
    }

    hRes = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hRes)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hRes = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT SerialNumber FROM Win32_PhysicalMemory"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
    if (FAILED(hRes)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    std::string info = "";
    while (pEnumerator) {
        hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        VARIANT vtProp;
        hRes = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);

        if (SUCCEEDED(hRes) && vtProp.vt == VT_BSTR) {
            info = wstring2string(vtProp.bstrVal);
        }
        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
    return info;
#elif defined(__linux__)
    char buffer[BUFFERLEN];
    FILE* pipe = popen("dmidecode -t memory | grep 'Serial Number'", "r");
    fgets(buffer, BUFFERLEN, pipe);
    pclose(pipe);
    std::string info = std::string(buffer);
    info = removeSubstring(info, "Serial Number");
    return info;
#elif defined(__APPLE__)
    char buffer[BUFFERLEN];
    FILE* pipe = popen("system_profiler SPMemoryDataType | grep 'Serial'", "r");
    fgets(buffer, BUFFERLEN, pipe);
    pclose(pipe);
    return std::string(buffer);
#endif 
}

std::string SerialNumber::GetGPUSerial() {
#if defined(_WIN32)
    DISPLAY_DEVICE dd;
    dd.cb  = sizeof(DISPLAY_DEVICE);
    EnumDisplayDevices(NULL, 0, &dd, 0);
    return dd.DeviceID;
#elif defined(__linux__)
    return "";
#elif defined(__APPLE__)
    return "";
#endif
}

std::string SerialNumber::GetDiskSerial() {
#if defined(_WIN32)
    FILE* pipe = _popen("wmic diskdrive get serialnumber", "r");
    if (!pipe) return "";

    char buffer[128];
    std::string info = "";
    while (fgets(buffer, sizeof(buffer), pipe)) {
        info += buffer;
    }
    _pclose(pipe);

    info = removeSubstring(info, "SerialNumber");
	info = removeSubstring(info, "\r\n");
    info = string_trim(info);
    return info;
#elif defined(__linux__)
    char buffer[BUFFERLEN];
    FILE* pipe = popen("udevadm info --query=all --name=/dev/sda | grep ID_SERIAL_SHORT", "r");
    fgets(buffer, BUFFERLEN, pipe);
    pclose(pipe);
    std::string info = std::string(buffer);
    info = removeSubstring(info, "E: ID_SERIAL_SHORT=");
    return info;
#elif defined(__APPLE__)
    char buffer[BUFFERLEN];
    FILE* pipe = popen("system_profiler SPStorageDataType | grep 'Volume UUID'", "r");
    fgets(buffer, BUFFERLEN, pipe);
    pclose(pipe);
    std::string info = std::string(buffer);
    info = removeSubstring(info, "Volume UUID: ");
    return info;
#endif
}
