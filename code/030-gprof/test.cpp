
#include <iostream>
using namespace std;
 
void func3(int depth) {
    for(int i=0; i<10000*(depth+1); i++); // 耗时操作 
}
 
void func2() {
    for(int i=0; i<50000; i++);
    func3(1);
}
 
void func1() {
    for(int i=0; i<100000; i++);
    func3(0);
    func2();
}
 
int main() {
    for(int i=0; i<5; i++){
        func1();
        func2();
    }
    return 0;
}