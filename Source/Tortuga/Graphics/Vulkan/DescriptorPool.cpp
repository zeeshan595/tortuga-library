#include "./DescriptorPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorPool
{
DescriptorPool Create(Device::Device device, DescriptorLayout::DescriptorLayout layout, uint32_t maxSets)
{
  DescriptorPool data = {};

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