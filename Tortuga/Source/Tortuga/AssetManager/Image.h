#ifndef _IMAGE_ASSET
#define _IMAGE_ASSET

#include "../Core.h"
#include "../Console.h"

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
  SDL_Surface *_imageSurface;
  ImageFormat _imageFormat;

public:
  Image(std::string filePath);
  Image(uint32_t width, uint32_t height);
  ~Image();

  SDL_Surface *GetSDLSurface() { return _imageSurface; }
};
}; // namespace AssetManager
}; // namespace Tortuga

#endif
