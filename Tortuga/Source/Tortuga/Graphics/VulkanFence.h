#ifndef _VULKAN_FENCE
#define _VULKAN_FENCE

#include "./VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanFence {
  VkFence Fence;
  VkDevice VirtualDevice;
};

VulkanFence CreateVulkanFence(VulkanDevice device);
void DestroyVulkanFence(VulkanFence data);
void FenceWait(std::vector<VulkanFence> data, bool waitAll = true,
               uint32_t timeout = std::numeric_limits<uint32_t>::max());
} // namespace Graphics
} // namespace Tortuga

#endif