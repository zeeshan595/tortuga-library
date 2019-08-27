#ifndef _VULKAN_IMAGE
#define _VULKAN_IMAGE

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Image
{
struct Image
{
  VkDevice Device;
  VkImage Image;
  VkDeviceMemory Memory;
  VkFormat Format;
  uint32_t Width;
  uint32_t Height;
};

//depth buffer helpers
VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat findDepthFormat(VkPhysicalDevice device);
bool hasStencilComponent(VkFormat format);

Image Create(Device::Device device, uint32_t width, uint32_t height, VkFormat imageFormat, VkImageUsageFlags usageFlags);
void Destroy(Image data);
} // namespace Image
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif