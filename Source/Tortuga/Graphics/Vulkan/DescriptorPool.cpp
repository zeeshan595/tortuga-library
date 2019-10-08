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

  std::unordered_map<VkDescriptorType, uint> BindingCounts;
  for (auto layout : layouts)
    for (auto type : layout.Types)
      BindingCounts[type]++;

  std::vector<VkDescriptorPoolSize> poolSizes;
  for (auto binding : BindingCounts)
  {
    if (binding.second > 0)
      poolSizes.push_back({binding.first, binding.second});
  }

  VkDescriptorPoolCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.maxSets = descriptorSetsCount;
    createInfo.poolSizeCount = poolSizes.size();
    createInfo.pPoolSizes = poolSizes.data();
  }
  ErrorCheck::Callback(vkCreateDescriptorPool(device.Device, &createInfo, nullptr, &data.Pool));
  return data;
}
void Destroy(DescriptorPool data)
{
  if (data.Pool == VK_NULL_HANDLE)
    return;

  vkDestroyDescriptorPool(data.Device, data.Pool, nullptr);
}
} // namespace DescriptorPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga