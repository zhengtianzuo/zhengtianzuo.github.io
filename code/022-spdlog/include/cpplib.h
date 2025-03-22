#pragma once

#undef cpplib_lib
#if defined(WIN32)
#define cpplib_atr
#if defined(cpplib_exports)
#define cpplib_lib _declspec(dllexport)
#else
#define cpplib_lib
#endif
#else
#define cpplib_atr __attribute__((visibility("default")))
#define cpplib_lib
#endif

class cpplib_lib cpplib {
public:
    cpplib();
    ~cpplib();

    static int add(int a, int b);
};

#ifdef __cplusplus
extern "C"
{
#endif

    cpplib_lib int cpplib_add(int a, int b);

#ifdef __cplusplus
}
#endif
