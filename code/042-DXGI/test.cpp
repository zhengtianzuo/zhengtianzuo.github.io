
#include <d3d11.h>
#include <dxgi1_2.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "User32.lib")

// 将RGB格式图片转成bmp格式
void saveAsBmp(const char *bmpFile,     // BMP文件名称
                          unsigned char *pRgbData, // 图像数据
                          int width,               // 图像宽度
                          int height,              // 图像高度
                          int biBitCount,          // 位图深度
                          bool flipvertical) // 图像是否需要垂直翻转
{
  int size = 0;
  int bitsPerPixel = 3;
  if (biBitCount == 24) {
    bitsPerPixel = 3;
    size = width * height * bitsPerPixel * sizeof(char); // 每个像素点3个字节
  } else if (biBitCount == 32) {
    bitsPerPixel = 4;
    size = width * height * bitsPerPixel * sizeof(char); // 每个像素点4个字节
  } else
    return;

  // 位图第一部分，文件信息
  BITMAPFILEHEADER bfh;
  bfh.bfType = (WORD)0x4d42; //图像格式 必须为'BM'格式
  bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //真正的数据的位置
  bfh.bfSize = size + bfh.bfOffBits;
  bfh.bfReserved1 = 0;
  bfh.bfReserved2 = 0;

  BITMAPINFOHEADER bih;
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = width;
  if (flipvertical)
    bih.biHeight = -height; // BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了
  else
    bih.biHeight = height;
  bih.biPlanes = 1;
  bih.biBitCount = biBitCount;
  bih.biCompression = BI_RGB;
  bih.biSizeImage = size;
  bih.biXPelsPerMeter = 0;
  bih.biYPelsPerMeter = 0;
  bih.biClrUsed = 0;
  bih.biClrImportant = 0;
  FILE *fp = NULL;
  fopen_s(&fp, bmpFile, "wb");
  if (!fp)
    return;

  fwrite(&bfh, 8, 1, fp);
  fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, fp);
  fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, fp);
  fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
  fwrite(pRgbData, size, 1, fp);
  fclose(fp);
}

int main() {
  
  // 获取屏幕尺寸
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  // 支持的驱动程序类型
  D3D_DRIVER_TYPE DriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE,};
  UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

  // 支持的功能级别
  D3D_FEATURE_LEVEL FeatureLevels[] = {
      D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_1};
  UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

  // 创建D3D设备
  D3D_FEATURE_LEVEL FeatureLevel;
  ID3D11Device *_pDX11Dev = nullptr;
  ID3D11DeviceContext *_pDX11DevCtx = nullptr;
  HRESULT hr = 0;
  for (UINT index = 0; index < NumDriverTypes; index++) {
    hr = D3D11CreateDevice(nullptr, DriverTypes[index], nullptr, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &_pDX11Dev, &FeatureLevel, &_pDX11DevCtx);
    if (SUCCEEDED(hr)) break;
  }

  // 获取DXGITest设备
  IDXGIDevice* _pDXGITestDev = nullptr;
  hr = _pDX11Dev->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&_pDXGITestDev));
  if (FAILED(hr)) {
    return false;
  }

  // 获取DXGITest适配器
  IDXGIAdapter* _pDXGITestAdapter = nullptr;
  hr = _pDXGITestDev->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void **>(&_pDXGITestAdapter));
  if (FAILED(hr)) {
    return false;
  }

  // 获取输出
  UINT i = 0;
  IDXGIOutput *_pDXGIOutput = nullptr;
  hr = _pDXGITestAdapter->EnumOutputs(i, &_pDXGIOutput);
  if (FAILED(hr)) {
    return false;
  }

  // 获取输出描述结构
  DXGI_OUTPUT_DESC DesktopDesc;
  _pDXGIOutput->GetDesc(&DesktopDesc);

  // 请求接口给Output1
  IDXGIOutput1 *_pDXGIOutput1 = nullptr;
  hr = _pDXGIOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void **>(&_pDXGIOutput1));
  if (FAILED(hr)) {
    return false;
  }

  // 创建桌面副本
  IDXGIOutputDuplication *_pDXGIOutputDup = nullptr;
  hr = _pDXGIOutput1->DuplicateOutput(_pDX11Dev, &_pDXGIOutputDup);
  if (FAILED(hr)) {
    return false;
  }

  for (int i = 0; i < 100; i++) {
    IDXGIResource *desktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    hr = _pDXGIOutputDup->AcquireNextFrame(20, &frameInfo, &desktopResource);
    if (FAILED(hr)) {
      if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
        if (desktopResource) {
          desktopResource->Release();
          desktopResource = nullptr;
          std::cout << " AcquireNextFrame超时" << std::endl;
          --i;
          continue;
        }
        hr = _pDXGIOutputDup->ReleaseFrame();
      } else {
          std::cout << " AcquireNextFrame错误: " << hr << std::endl;
        return false;
      }
    }
    if (desktopResource == nullptr) {
        std::cout << " AcquireNextFrame超时" << std::endl;
        --i;
        continue;
    }

    // 查询下一帧暂存缓冲区
    ID3D11Texture2D *_pDX11Texture = nullptr;
    hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&_pDX11Texture));
    desktopResource->Release();
    desktopResource = nullptr;
    if (FAILED(hr)) {
      return false;
    }

    // 复制旧描述
    ID3D11Texture2D *_pCopyBuffer = nullptr;
    D3D11_TEXTURE2D_DESC desc;
    if (_pDX11Texture) {
      _pDX11Texture->GetDesc(&desc);
    } else if (_pCopyBuffer) {
      _pCopyBuffer->GetDesc(&desc);
    } else {
        std::cout << " GetDesc错误" << std::endl;
      return false;
    }

    // 为填充帧图像创建一个新的暂存缓冲区
    if (_pCopyBuffer == nullptr) {
      D3D11_TEXTURE2D_DESC CopyBufferDesc;
      CopyBufferDesc.Width = desc.Width;
      CopyBufferDesc.Height = desc.Height;
      CopyBufferDesc.MipLevels = 1;
      CopyBufferDesc.ArraySize = 1;
      CopyBufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      CopyBufferDesc.SampleDesc.Count = 1;
      CopyBufferDesc.SampleDesc.Quality = 0;
      CopyBufferDesc.Usage = D3D11_USAGE_STAGING;
      CopyBufferDesc.BindFlags = 0;
      CopyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
      CopyBufferDesc.MiscFlags = 0;

      hr = _pDX11Dev->CreateTexture2D(&CopyBufferDesc, nullptr, &_pCopyBuffer);
      if (FAILED(hr)) {
          std::cout << " CreateTexture2D错误: " << hr << std::endl;
        return false;
      }
    }

    if (_pDX11Texture) {
      // 将下一个暂存缓冲区复制到新的暂存缓冲区
      _pDX11DevCtx->CopyResource(_pCopyBuffer, _pDX11Texture);
    }

    // 为映射位创建暂存缓冲区
    IDXGISurface* CopySurface = nullptr;
    hr = _pCopyBuffer->QueryInterface(__uuidof(IDXGISurface), (void **)&CopySurface);
    if (FAILED(hr)) {
        std::cout << " QueryInterface错误: " << hr << std::endl;
      return false;
    }

    // 将位复制到用户空间
    DXGI_MAPPED_RECT MappedSurface;
    hr = CopySurface->Map(&MappedSurface, DXGI_MAP_READ);

    //保存为bmp格式
    char picNameB[128] = {0};
    snprintf(picNameB, sizeof(picNameB), "Screen%d.bmp", i);
    saveAsBmp(picNameB, MappedSurface.pBits, screenWidth, screenHeight, 32, true);

    std::cout << " 完成: " << i << std::endl;

    CopySurface->Unmap();
    hr = CopySurface->Release();
    CopySurface = nullptr;

    if (_pDXGIOutputDup) {
      hr = _pDXGIOutputDup->ReleaseFrame();
    }
  }

  return 0;
}
