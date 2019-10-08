#ifndef _GRAPHICS_IMAGE
#define _GRAPHICS_IMAGE

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

#include "./Pixel.hpp"

namespace Tortuga
{
namespace Graphics
{
struct Image
{
  uint32_t Width;
  uint32_t Height;
  uint32_t Channels;
  uint32_t ByteSize;
  std::vector<Pixel> Pixels;

  Image()
  {
    Width = 1;
    Height = 1;
    Channels = 4;
    ByteSize = 4;
    Pixels = {{1, 0, 0, 1}};
  }
  Image(uint32_t width, uint32_t height)
  {
    Width = width;
    Height = height;
    Channels = 4;
    ByteSize = width * height * Channels;
    Pixels.resize(height * width);
  }
};
} // namespace Graphics
} // namespace Tortuga

#endif