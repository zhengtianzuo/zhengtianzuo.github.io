#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <png.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

// sudo apt-get install libxcb-image0 libxcb-image0-dev libxcb1-dev libx11-xcb-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-util-dev libpng-dev

void write_png(const char* filename, uint8_t* data, int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    // 设置PNG参数
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    // 分配行指针
    png_bytep* row_pointers = new png_bytep[height];
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = data + y * width * 3;
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // 清理资源
    delete[] row_pointers;
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    // 连接X服务器
    xcb_connection_t* conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) {
        fprintf(stderr, "连接X服务器失败\n");
        return 1;
    }

    // 获取第一个屏幕
    const xcb_setup_t* setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t* screen = iter.data;

    // 获取屏幕尺寸
    uint16_t width = screen->width_in_pixels;
    uint16_t height = screen->height_in_pixels;

    // 获取屏幕图像
    xcb_get_image_cookie_t cookie = xcb_get_image(
        conn,
        XCB_IMAGE_FORMAT_Z_PIXMAP,
        screen->root,
        0, 0,
        width, height,
        ~0
    );

    xcb_get_image_reply_t* reply = xcb_get_image_reply(conn, cookie, NULL);
    if (!reply) {
        fprintf(stderr, "获取图像失败\n");
        xcb_disconnect(conn);
        return 1;
    }

    uint8_t* xcb_data = xcb_get_image_data(reply);
    uint32_t data_len = xcb_get_image_data_length(reply);

    FILE* file = fopen("screenshot.rgb", "wb+");
    if (file != nullptr) {
        fwrite(xcb_data, data_len, 1, file);
        fflush(file);
        fclose(file);
    }

    // 转换颜色格式（BGRX -> RGB）
    uint8_t* rgb_data = new uint8_t[width * height * 3];
    for (uint32_t i = 0; i < width * height; ++i) {
        // 假设每个像素4字节（BGRX格式）
        rgb_data[i*3 + 0] = xcb_data[i*4 + 2];  // R
        rgb_data[i*3 + 1] = xcb_data[i*4 + 1];  // G
        rgb_data[i*3 + 2] = xcb_data[i*4 + 0];  // B
    }

    // 保存为PNG
    write_png("screenshot.png", rgb_data, width, height);

    // 清理资源
    delete[] rgb_data;
    free(reply);
    xcb_disconnect(conn);

    return 0;
}
