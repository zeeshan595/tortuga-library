#include "DescriptorPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorPoolData CreateDescriptorPool(DeviceData device, VkDescriptorType type, uint32_t setSize, uint32_t descriptorSize)
{
  auto data = DescriptorPoolData();
  data.Device = device.Device;
  data.Type = type;
  data.DescriptorSetSize = setSize;
  data.DescriptorSize = descriptorSize;

  auto poolSize = VkDescriptorPoolSize();
  {
    poolSize.type = type;
    poolSize.descriptorCount = data.DescriptorSize * data.DescriptorSetSize;
  }

  auto poolInfo = VkDescriptorPoolCreateInfo();
  {
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = data.DescriptorSetSize;
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