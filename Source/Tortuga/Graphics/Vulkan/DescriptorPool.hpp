#ifndef _VULKAN_DESCRIPTOR_POOL
#define _VULKAN_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorPool
{
struct DescriptorPool
{
  VkDevice Device;
  VkDescriptorPool Pool;
  uint32_t DescriptorSetCounts;
};

DescriptorPool Create(Device::Device device, DescriptorLayout::DescriptorLayout layout, uint32_t maxSets = 1)
{
  DescriptorPool data = {};
  data.DescriptorSetCounts = maxSets;

  VkDescriptorPoolCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = maxSets;
    createInfo.poolSizeCount = layout.PoolSizes.size();
    createInfo.pPoolSizes = layout.PoolSizes.data();
  }
  ErrorCheck::Callback(vkCreateDescriptorPool(device.Device, &createInfo, nullptr, &data.Pool));
  return data;
}
void Destroy(DescriptorPool data)
{
  vkDestroyDescriptorPool(data.Device, data.Pool, nullptr);
}
} // namespace DescriptorPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif