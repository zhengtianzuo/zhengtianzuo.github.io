#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct YUVConfig {
    int width;      // 大文件宽度 
    int height;     // 大文件高度 
    int small_width;
    int small_height;
    int pos_x;      // 覆盖起始X坐标 
    int pos_y;      // 覆盖起始Y坐标 
    std::string format = "I420"; // 支持I420/NV12格式 
};

bool overlayYUV(char* dst_data, char* src_data, const YUVConfig& config) 
{
    // 计算各分量尺寸 
    const int y_size = config.width * config.height; 
    const int uv_size = y_size / 4;
    const int total_size = y_size + 2 * uv_size;

    // Y分量覆盖处理 
    const int copy_width = std::min(config.small_width, config.width  - config.pos_x); 
    const int copy_height = std::min(config.small_height, config.height  - config.pos_y); 
    
    // 处理Y平面 
    for (int row = 0; row < copy_height; ++row) {
        auto* dst_start = dst_data + (config.pos_y  + row) * config.width  + config.pos_x; 
        auto* src_start = src_data + row * config.small_width; 
        std::copy_n(src_start, copy_width, dst_start);
    }

    // UV分量处理（I420格式）
    if (config.format  == "I420") {
        // U分量偏移计算 
        const int uv_offset = y_size;
        const int uv_pos_x = config.pos_x  / 2;
        const int uv_pos_y = config.pos_y  / 2;
        const int uv_src_width = config.small_width  / 2;
        const int uv_src_height = config.small_height  / 2;

        // 处理U平面 
        for (int row = 0; row < uv_src_height; ++row) {
            auto* dst_u = dst_data + uv_offset + (uv_pos_y + row) * (config.width/2)  + uv_pos_x;
            auto* src_u = src_data + y_size + row * uv_src_width;
            std::copy_n(src_u, uv_src_width, dst_u);
        }

        // 处理V平面 
        const int v_offset = y_size + uv_size;
        for (int row = 0; row < uv_src_height; ++row) {
            auto* dst_v = dst_data + v_offset + (uv_pos_y + row) * (config.width/2)  + uv_pos_x;
            auto* src_v = src_data + y_size + uv_src_width*uv_src_height + row * uv_src_width;
            std::copy_n(src_v, uv_src_width, dst_v);
        }
    }
    // NV12格式处理（UV交织）
    else if (config.format  == "NV12") {
        const int uv_offset = y_size;
        const int uv_pos_x = config.pos_x  / 2;
        const int uv_pos_y = config.pos_y  / 2;
        const int uv_stride = config.width; 

        for (int row = 0; row < config.small_height/2;  ++row) {
            auto* dst_uv = dst_data + uv_offset + (uv_pos_y + row) * uv_stride + uv_pos_x*2;
            auto* src_uv = src_data + y_size + row * config.small_width; 
            std::copy_n(src_uv, config.small_width,  dst_uv);
        }
    }
    return true;
}

int main() {
    YUVConfig config;
    config.width  = 352;
    config.height  = 288;
    config.small_width  = 176;
    config.small_height  = 144;
    config.pos_x  = 10;
    config.pos_y  = 20;
    config.format  = "I420"; // 或"NV12"

    // 计算各分量尺寸 
    const int y_size = config.width  * config.height; 
    const int uv_size = y_size / 4;
    const int total_size = y_size + 2 * uv_size;

    // 读取大文件 
    std::ifstream fin_dst("big.yuv", std::ios::binary);
    if (!fin_dst) return false;
    char *dst_data = new char[total_size];
    fin_dst.read(dst_data,  total_size);

    // 读取小文件 
    std::ifstream fin_src("small.yuv", std::ios::binary);
    if (!fin_src) return false;
    int src_data_len = y_size + 2*(config.small_width*config.small_height)/4;
    char *src_data = new char[src_data_len]; 
    fin_src.read(src_data, src_data_len); 
    
    if (overlayYUV(dst_data,  src_data,  config)) {
        // 写入结果文件 
        std::ofstream fout("output.yuv",  std::ios::binary);
        fout.write(dst_data, total_size); 

        std::cout << "Overlay completed. Saved to output.yuv"  << std::endl;
    } else {
        std::cerr << "Error processing files" << std::endl;
    }
    return 0;
}
