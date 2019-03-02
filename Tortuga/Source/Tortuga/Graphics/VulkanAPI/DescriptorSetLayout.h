#ifndef _VULKAN_API_DESCRIPTOR_SET_LAYOUT
#define _VULKAN_API_DESCRIPTOR_SET_LAYOUT

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorLayoutData CreateDescriptorSetLayout(DeviceData device);
void DestroyDescriptorSetLayout(DescriptorLayoutData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif