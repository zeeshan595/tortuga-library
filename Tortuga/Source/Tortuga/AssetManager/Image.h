#ifndef _IMAGE_ASSET
#define _IMAGE_ASSET

#include "../Core.h"
#include "../Console.h"

#include "../Core/Pixel.h"

namespace Tortuga
{
namespace AssetManager
{
struct ImageData
{
  Image::ImageFileFormat FileFormat;
  uint32_t Width;
  uint32_t Height;
  std::vector<Pixel> Pixels;
};

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
ImageData LoadImage(std::string file);
}; // namespace Image
}; // namespace AssetManager
}; // namespace Tortuga

#endif
