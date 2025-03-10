#include <iostream>
#include <random>
#include <unordered_set>
#include <chrono>
 
using namespace std;
 
const int TARGET_LENGTH = 9;
const long long MIN_VALUE = 100'000'000;  // 9位最小值 
const long long MAX_VALUE = 999'999'999;  // 9位最大值 
 
// 生成指定数量的唯一9位随机数
unordered_set<long long> generate_unique_numbers(int quantity) {
    unordered_set<long long> unique_numbers;
    random_device rd;
    mt19937_64 generator(rd());  // 使用64位梅森旋转算法 
    uniform_int_distribution<long long> distribution(MIN_VALUE, MAX_VALUE);
 
    while (unique_numbers.size()  < quantity) {
        long long num = distribution(generator);
        if (num < MIN_VALUE) continue;  // 确保最小值 
        
        // 检查数字长度和唯一性
        if (to_string(num).length() == TARGET_LENGTH) {
            auto result = unique_numbers.insert(num); 
            if (result.second)  {
                cout << "生成成功: " << num << endl;
            }
        }
    }
    return unique_numbers;
}
 
int main() {
    try {
        cout << "请输入需要生成的随机数数量: ";
        int count;
        cin >> count;
 
        // 有效性验证 
        if (count <= 0 || count > (MAX_VALUE - MIN_VALUE + 1)) {
            cerr << "错误：请求数量超出有效范围" << endl;
            return EXIT_FAILURE;
        }
 
        auto start = chrono::high_resolution_clock::now();
        auto numbers = generate_unique_numbers(count);
        auto end = chrono::high_resolution_clock::now();
 
        // 输出统计信息 
        chrono::duration<double> duration = end - start;
        cout << "\n生成统计:"
             << "\n成功数量: " << numbers.size() 
             << "\n耗时: " << duration.count()  << "秒"
             << "\n生成效率: " << numbers.size()/duration.count()  << "个/秒" 
             << endl;
 
    } catch (const exception& e) {
        cerr << "运行时错误: " << e.what()  << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
