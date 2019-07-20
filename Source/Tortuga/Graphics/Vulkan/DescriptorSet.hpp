#ifndef _VULKAN_DESCRIPTOR_SET
#define _VULKAN_DESCRIPTOR_SET

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./DescriptorPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorSet
{
struct DescriptorSet
{
  VkDevice Device;
  VkDescriptorSet set;
  VkDescriptorPool Pool;
  uint32_t DescriptorSetCount;
};

DescriptorSet Create(Device::Device device, DescriptorLayout::DescriptorLayout layout, DescriptorPool::DescriptorPool pool);
void Destroy(DescriptorSet data);
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif