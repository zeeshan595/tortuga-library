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
void ConfigureDescriptorSetSampledImage(DescriptorSetData data, VkImageLayout imageLayout, VkImageView imageView, VkSampler imageSampler, uint32_t setIndex, uint32_t binding);
void ConfigureDescriptorSetBuffer(DescriptorSetData data, BufferData buffer, uint32_t setIndex, uint32_t binding);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif