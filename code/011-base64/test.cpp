#include <string>
#include <vector>
#include <iostream>

const std::string BASE64_CHARS = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const std::string &input) {
    std::string encoded;
    int val = 0, valb = -6;
    unsigned int len = input.length(); 
    
    for (unsigned char c : input) {
        val = (val << 8) + c;  // 合并3字节到24位缓冲区 
        valb += 8;
        while (valb >= 0) {    // 每次提取6位生成编码字符 
            encoded.push_back(BASE64_CHARS[(val  >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    // 处理剩余位
    if (valb > -6) {
        encoded.push_back(BASE64_CHARS[((val  << 8) >> (valb + 8)) & 0x3F]);
    }
    
    // 填充等号 
    while (encoded.size()  % 4) {
        encoded.push_back('='); 
    }
    
    return encoded;
}

std::string base64_decode(const std::string &input) {
    std::vector<int> decode_table(256, -1);
    for (int i = 0; i < 64; i++) {
        decode_table[BASE64_CHARS[i]] = i;
    }

    std::string decoded;
    int val = 0, bits = -8;
    unsigned int len = input.size(); 
    
    for (unsigned char c : input) {
        if (c == '=') break; // 停止处理填充符
        if (decode_table[c] == -1) continue; // 跳过非法字符
        
        val = (val << 6) + decode_table[c]; // 合并6位数据到24位缓冲区
        bits += 6;
        
        if (bits >= 0) { // 每次提取8位生成原始字节
            decoded.push_back((val  >> bits) & 0xFF);
            bits -= 8;
        }
    }
    
    // 处理剩余位（根据=数量判断有效数据）
    if (bits > -8 && (input.back()  != '=' || (len % 4) == 1)) {
        decoded.push_back((val  << 8) >> (bits + 8) & 0xFF);
    }
    
    return decoded;
}

int main() {
    std::string plain = "Hello World!";
    std::string encoded = base64_encode(plain);
    // 输出：SGVsbG8gV29ybGQh
    std::cout << encoded << std::endl; 

    std::string decoded = base64_decode(encoded);
    // 输出：Hello World!
    std::cout << decoded << std::endl; 
    return 0;
}
