#ifndef _VULKAN_API_DESCRIPTOR_POOL
#define _VULKAN_API_DESCRIPTOR_POOL

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorPoolData CreateDescriptorPool(DeviceData device, VkDescriptorType type, uint32_t size);
void DestroyDescriptorPool(DescriptorPoolData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif