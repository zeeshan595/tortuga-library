#ifndef _VULKAN_IMAGE
#define _VULKAN_IMAGE

#include "./VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanImage {
  uint32_t Width;
  uint32_t Height;
  VkImage Image;
  VkFormat ImageFormat;
  VkDevice VirtualDevice;
  VkDeviceMemory DeviceMemory;
};

VulkanImage CreateVulkanImage(VulkanDevice device, uint32_t width,
                              uint32_t height);
void DestroyVulkanImage(VulkanImage data);
} // namespace Graphics
} // namespace Tortuga

#endif