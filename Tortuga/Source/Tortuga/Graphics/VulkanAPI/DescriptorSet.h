#ifndef _DESCRIPTOR_SET
#define _DESCRIPTOR_SET

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorSetData CreateDescriptorSet(DeviceData device, DescriptorLayoutData layout);
void DestroyDescriptorSet(DescriptorSetData data);
void UpdateDescriptorSetBuffer(DescriptorSetData data, BufferData buffer);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif