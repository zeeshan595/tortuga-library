#include "DescriptorPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorPoolData CreateDescriptorPool(DeviceData device, VkDescriptorType type, uint32_t size)
{
  auto data = DescriptorPoolData();
  data.Type = type;
  data.PoolSize = size;

  auto poolSize = VkDescriptorPoolSize();
  {
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = size;
  }

  auto poolInfo = VkDescriptorPoolCreateInfo();
  {
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = size;
  }
  if (vkCreateDescriptorPool(device.Device, &poolInfo, nullptr, &data.Pool) != VK_SUCCESS)
  {
    Console::Error("Failed to create descriptor pool on device: {0}", device.Properties.deviceName);
  }

  return data;
}
void DestroyDescriptorPool(DescriptorPoolData data)
{
  vkDestroyDescriptorPool(data.Device, data.Pool, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga