#include <stdlib.h>

void leak() {
    int* ptr = new int[10](); // 内存泄漏
}

void uninit() {
    int x;
    if(x > 0) { // 未初始化使用
        //...
    }
}

int main() {
    int* arr = (int*)malloc(sizeof(int)*20);
    arr[20]  = 0; // 越界访问
    leak();
    uninit();
    free(arr);
    return 0;
}