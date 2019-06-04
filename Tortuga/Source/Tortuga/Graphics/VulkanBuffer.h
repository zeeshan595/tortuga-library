#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include "./VulkanErrorHandler.h"
#include "./VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanBuffer {
  VkBuffer Buffer;
  VkDeviceMemory Memory;
  VkDevice VirtualDevice;
};
VulkanBuffer CreateVulkanBuffer(VulkanDevice device, uint32_t bufferSize);
void DestroyVulkanBuffer(VulkanBuffer data);
} // namespace Graphics
} // namespace Tortuga

#endif