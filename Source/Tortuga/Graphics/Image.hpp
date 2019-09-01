#ifndef _GRAPHICS_IMAGE
#define _GRAPHICS_IMAGE

#include <cstdint>
#include <vulkan/vulkan.h>

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
  void* Pixels;

  Image()
  {
    Width = 0;
    Height = 0;
    Channels = 0;
    ByteSize = 0;
    Pixels = VK_NULL_HANDLE;
  }
};
} // namespace Graphics
} // namespace Tortuga

#endif