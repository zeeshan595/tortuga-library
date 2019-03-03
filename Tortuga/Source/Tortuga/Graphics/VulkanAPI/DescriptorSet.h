#ifndef _VULKAN_API_DESCRIPTOR_SET
#define _VULKAN_API_DESCRIPTOR_SET

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorSetData ConfigureDescriptorSets(DeviceData device, DescriptorLayoutData layout, DescriptorPoolData pool);
void ConfigureDescriptorSetBuffer(DescriptorSetData data, BufferData buffer, uint32_t binding, uint32_t setIndex);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif