#include "./DescriptorPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorPool
{
DescriptorPool Create(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, uint32_t descriptorSetsCount)
{
  DescriptorPool data = {};
  data.Device = device.Device;
  data.DescriptorSetCounts = descriptorSetsCount;

  uint32_t bindingsCount = 0;
  for (auto layout : layouts)
    bindingsCount += layout.BindingsAmount;

  VkDescriptorPoolSize poolSize = {};
  poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  poolSize.descriptorCount = bindingsCount;

  VkDescriptorPoolCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = descriptorSetsCount;
    createInfo.poolSizeCount = 1;
    createInfo.pPoolSizes = &poolSize;
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