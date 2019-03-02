#ifndef _VULKAN_API_DESCRIPTOR_SET
#define _VULKAN_API_DESCRIPTOR_SET

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorSetData CreateDescriptorSet(DeviceData device, DescriptorLayoutData layout, DescriptorPoolData pool);
void DestroyDescriptorSet(DescriptorSetData data);
void ConfigureDescriptorSetBuffer(DescriptorSetData data, BufferData buffer, uint32_t binding);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif