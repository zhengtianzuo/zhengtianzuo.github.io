#include <windows.h>
#include <detours.h>
#include <iostream>
 
// 原始API函数指针声明 
static HANDLE (WINAPI *TrueCreateFileW)(
    LPCWSTR lpFileName, DWORD dwDesiredAccess,
    DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) = CreateFileW;
 
static int (WINAPI *TrueMessageBoxW)(
    HWND hWnd, LPCWSTR lpText, 
    LPCWSTR lpCaption, UINT uType) = MessageBoxW;
 
// 自定义处理函数 
HANDLE WINAPI HookedCreateFileW(
    LPCWSTR lpFileName, DWORD dwDesiredAccess,
    DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    // 过滤敏感文件访问 
    if (wcsstr(lpFileName, L"secret.txt"))  {
        SetLastError(ERROR_ACCESS_DENIED);
        return INVALID_HANDLE_VALUE;
    }
    return TrueCreateFileW(lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition, 
        dwFlagsAndAttributes, hTemplateFile);
}
 
int WINAPI HookedMessageBoxW(
    HWND hWnd, LPCWSTR lpText,
    LPCWSTR lpCaption, UINT uType)
{
    // 修改弹窗内容 
    return TrueMessageBoxW(hWnd, L"内容已被拦截", 
        L"安全警告", MB_ICONWARNING);
}
 
// 钩子管理模块 
class DetourManager {
public:
    static void InstallHook() {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        DetourAttach(&(PVOID&)TrueCreateFileW, HookedCreateFileW);
        DetourAttach(&(PVOID&)TrueMessageBoxW, HookedMessageBoxW);
 
        DetourTransactionCommit();
    }
 
    static void RemoveHook() {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        
        DetourDetach(&(PVOID&)TrueCreateFileW, HookedCreateFileW);
        DetourDetach(&(PVOID&)TrueMessageBoxW, HookedMessageBoxW);
 
        DetourTransactionCommit();
    }
};
 
// DLL注入支持 
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DetourManager::InstallHook();
        break;
    case DLL_PROCESS_DETACH:
        DetourManager::RemoveHook();
        break;
    }
    return TRUE;
}
 
// 控制台测试程序 
int main() {
    DetourManager::InstallHook();
 
     // 测试文件访问拦截 
    HANDLE hFile = CreateFileW(L"secret.txt",  GENERIC_READ, 0, 
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "文件访问拦截成功\n";
    }
	
    // 测试弹窗拦截 
    MessageBoxW(NULL, L"原始内容", L"测试弹窗", MB_OK);
 
    DetourManager::RemoveHook();
    return 0;
}
