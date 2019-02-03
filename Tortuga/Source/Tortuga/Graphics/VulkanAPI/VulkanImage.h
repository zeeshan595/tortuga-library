#ifndef _VULKAN_IMAGE
#define _VULKAN_IMAGE

#include "../../Core.h"
#include "../../Console.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class VulkanImage
{
  private:
    VkDeviceSize *_imageSize;
    VkImage *_image;

  public:
    VulkanImage();
    ~VulkanImage();
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif