#include <iostream>
#include <fstream>
#include <vector>
#include <zstd.h>

const int BLOCK_SIZE = 1024 * 1024; // 设置块大小为1MB

// 压缩函数 
bool zstd_compress(const char* input_path, const char* output_path, int level=3) {
    std::ifstream fin(input_path, std::ios::binary);
    std::vector<char> input((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    
    size_t bound = ZSTD_compressBound(input.size()); 
    std::vector<char> output(bound);
    
    ZSTD_CCtx* cctx = ZSTD_createCCtx();
    size_t csize = ZSTD_compressCCtx(cctx, 
        output.data(),  bound, 
        input.data(),  input.size(),  
        level);
    
    if (ZSTD_isError(csize)) {
        ZSTD_freeCCtx(cctx);
        return false;
    }
    
    std::ofstream fout(output_path, std::ios::binary);
    fout.write(output.data(),  csize);
    
    ZSTD_freeCCtx(cctx);
    return true;
}

// 解压函数 
bool zstd_decompress(const char* input_path, const char* output_path) {
    std::ifstream fin(input_path, std::ios::binary);
    std::vector<char> input((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    
    unsigned long long rsize = ZSTD_getFrameContentSize(input.data(),  input.size()); 
    std::vector<char> output(rsize);
    
    ZSTD_DCtx* dctx = ZSTD_createDCtx();
    size_t dsize = ZSTD_decompressDCtx(dctx, 
        output.data(),  rsize, 
        input.data(),  input.size()); 
    
    if (ZSTD_isError(dsize)) {
        ZSTD_freeDCtx(dctx);
        return false;
    }
    
    std::ofstream fout(output_path, std::ios::binary);
    fout.write(output.data(),  dsize);
    
    ZSTD_freeDCtx(dctx);
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
        if (zstd_compress(input.c_str(), output.c_str())) {
            std::cout << "文件压缩成功" << std::endl;
        } else {
            std::cerr << "文件压缩失败" << std::endl;
            return 1;
        }
    } else if (mode == "解压") {
        if (zstd_decompress(input.c_str(), output.c_str())) {
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
