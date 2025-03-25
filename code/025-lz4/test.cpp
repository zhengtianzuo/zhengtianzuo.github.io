#include <iostream>
#include <fstream>
#include <vector>
#include <lz4.h>

const int BLOCK_SIZE = 1024 * 1024; // 设置块大小为1MB

// 文件压缩函数
bool compressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    if (!inputFile) {
        std::cerr << "无法打开输入文件进行压缩" << std::endl;
        return false;
    }

    std::streamsize fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "无法打开输出文件进行写入" << std::endl;
        return false;
    }

    std::vector<char> inputBuffer(BLOCK_SIZE);
    std::vector<char> outputBuffer(LZ4_compressBound(BLOCK_SIZE));

    while (fileSize > 0) {
        inputFile.read(&inputBuffer[0], BLOCK_SIZE);
        size_t bytesRead = inputFile.gcount();
        if (bytesRead == 0)  {
            std::cerr << "readsome 0" << std::endl;
            break;
        }

        int compressedSize = LZ4_compress_default(&inputBuffer[0], &outputBuffer[0], bytesRead, outputBuffer.size());
        if (compressedSize == 0) {
            std::cerr << "压缩失败" << std::endl;
            return false;
        }

        outputFile.write(&outputBuffer[0], compressedSize);
        fileSize -= bytesRead;
    }

    return true;
}

// 文件解压函数
bool decompressFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "无法打开输入文件进行解压" << std::endl;
        return false;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "无法打开输出文件进行写入" << std::endl;
        return false;
    }

    std::vector<char> inputBuffer(BLOCK_SIZE);
    std::vector<char> outputBuffer(BLOCK_SIZE);

    while (true) {
        inputFile.read(&inputBuffer[0], BLOCK_SIZE);
        size_t bytesRead = inputFile.gcount();
        if (bytesRead == 0) break;

        int decompressedSize = LZ4_decompress_safe(&inputBuffer[0], &outputBuffer[0], bytesRead, BLOCK_SIZE);
        if (decompressedSize < 0) {
            std::cerr << "解压失败" << std::endl;
            return false;
        }

        outputFile.write(&outputBuffer[0], decompressedSize);
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "用法: " << argv[0] << " <压缩|解压> <输入文件> <输出文件>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    if (mode == "压缩") {
        if (compressFile(input, output)) {
            std::cout << "文件压缩成功" << std::endl;
        } else {
            std::cerr << "文件压缩失败" << std::endl;
            return 1;
        }
    } else if (mode == "解压") {
        if (decompressFile(input, output)) {
            std::cout << "文件解压成功" << std::endl;
        } else {
            std::cerr << "文件解压失败" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "无效的模式，请选择压缩或解压" << std::endl;
        return 1;
    }

    return 0;
}
