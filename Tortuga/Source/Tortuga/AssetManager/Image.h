#ifndef _IMAGE_ASSET
#define _IMAGE_ASSET

#include "../Core.h"
#include "../Console.h"

#include "Pixel.h"

namespace Tortuga
{
namespace AssetManager
{
class Image
{
public:
  enum ImageFormat
  {
    PNG,
    JPG,
    BMP,
    GIF,
    ICO,
    CUR,
    LBM,
    PCX,
    PNM,
    TIF,
    WEBP,
    XCF,
    XV,
    UNKNOWN
  };

private:
  ImageFormat _imageFileFormat;
  uint32_t _width;
  uint32_t _height;
  std::vector<Pixel> _pixels;

public:
  Image(std::string filePath);
  Image(uint32_t width, uint32_t height);
  ~Image();

  ImageFormat GetFileFormat() { return _imageFileFormat; }
  uint32_t GetWidth() { return _width; }
  uint32_t GetHeight() { return _height; }
  uint32_t GetPixelSize() { return sizeof(Pixel); }
  uint32_t GetPixelsBytesSize() { return _width * _height * sizeof(Pixel); }
  std::vector<Pixel> GetPixels() { return _pixels; }
};
}; // namespace AssetManager
}; // namespace Tortuga

#endif
