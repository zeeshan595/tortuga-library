#ifndef _IMAGE_ASSET
#define _IMAGE_ASSET

#include "../Core.h"
#include "../Console.h"

namespace Tortuga
{
struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};
namespace Assets
{
namespace Image
{
enum ImageFileFormat
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
struct ImageData
{
  ImageFileFormat FileFormat;
  uint32_t Width;
  uint32_t Height;
  std::vector<Pixel> Pixels;
};
ImageData LoadImage(std::string file);
}; // namespace Image
}; // namespace Assets
}; // namespace Tortuga

#endif
