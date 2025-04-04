#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")

int main() {

  // 获取屏幕DC
  HDC hScreenDC = GetDC(NULL);

  // 获取屏幕尺寸
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  // 创建兼容的DC
  HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

  // 创建位图
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);

  // 将位图与兼容的DC关联
  SelectObject(hMemoryDC, hBitmap);

  // 获取位图信息
  BITMAPINFO bmpInfo;
  bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth = screenWidth;
  bmpInfo.bmiHeader.biHeight = -screenHeight;
  bmpInfo.bmiHeader.biPlanes = 1;
  bmpInfo.bmiHeader.biBitCount = 24;
  bmpInfo.bmiHeader.biCompression = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage = 0;
  bmpInfo.bmiHeader.biXPelsPerMeter = 0;
  bmpInfo.bmiHeader.biYPelsPerMeter = 0;
  bmpInfo.bmiHeader.biClrUsed = 0;
  bmpInfo.bmiHeader.biClrImportant = 0;

  // 创建BMP文件头
  BITMAPFILEHEADER bmpFileHeader;
  bmpFileHeader.bfType = 0x4D42;
  bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + screenWidth * screenHeight * 3;
  bmpFileHeader.bfReserved1 = 0;
  bmpFileHeader.bfReserved2 = 0;
  bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  BYTE *pixels = new BYTE[screenWidth * screenHeight * 3];

  char strName[_MAX_FNAME];
  for (int i = 0; i < 100; i++) {

    // 将屏幕内容拷贝到位图中
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    // 获取位图像素数据
    GetDIBits(hMemoryDC, hBitmap, 0, screenHeight, pixels, &bmpInfo, DIB_RGB_COLORS);

    // 创建文件并写入BMP文件头和像素数据
    sprintf_s(strName, _MAX_FNAME, "screenshot%d.bmp", i);
    FILE* file = fopen(strName, "wb");
    fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, file);
    fwrite(pixels, screenWidth * screenHeight * 3, 1, file);
    fclose(file);

    std::cout << " 完成: " << i << std::endl;
  }

  // 释放资源
  delete[] pixels;
  DeleteObject(hBitmap);
  DeleteDC(hMemoryDC);
  ReleaseDC(NULL, hScreenDC);
  return 0;
}
