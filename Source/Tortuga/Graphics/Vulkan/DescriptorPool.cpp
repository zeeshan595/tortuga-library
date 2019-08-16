#include "./DescriptorPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorPool
{
DescriptorPool Create(Device::Device device, uint32_t descriptorSetsCount, std::vector<uint32_t> bindingsAmount)
{
  DescriptorPool data = {};
  data.Device = device.Device;
  data.DescriptorSetCounts = descriptorSetsCount;

  std::vector<VkDescriptorPoolSize> poolSize(bindingsAmount.size());
  for (uint32_t i = 0; i < bindingsAmount.size(); i++)
  {
    poolSize[i].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSize[i].descriptorCount = bindingsAmount[i];
  }
  data.PoolSizes = poolSize;

  VkDescriptorPoolCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = descriptorSetsCount;
    createInfo.poolSizeCount = poolSize.size();
    createInfo.pPoolSizes = poolSize.data();
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