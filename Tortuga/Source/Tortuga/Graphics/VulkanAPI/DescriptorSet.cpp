#include "DescriptorSet.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void ConfigureDescriptorSetBuffer(DescriptorSetData data, BufferData buffer, uint32_t binding)
{
  auto bufferInfo = VkDescriptorBufferInfo();
  {
    bufferInfo.buffer = buffer.Buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = buffer.BufferSize;
  }

  auto descriptorWrite = VkWriteDescriptorSet();
  {
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = data.DescriptorSet;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;       // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional
  }
  vkUpdateDescriptorSets(data.Device, 1, &descriptorWrite, 0, nullptr);
}
DescriptorSetData CreateDescriptorSet(DeviceData device, DescriptorLayoutData layout, DescriptorPoolData pool)
{
  auto data = DescriptorSetData();
  data.Device = device.Device;

  auto allocInfo = VkDescriptorSetAllocateInfo();
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool.Pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout.Layout;
  }
  if (vkAllocateDescriptorSets(data.Device, &allocInfo, &data.DescriptorSet) != VK_SUCCESS)
  {
    Console::Fatal("Failed to allocate descriptor sets on device: {0}", device.Properties.deviceName);
  }
  return data;
}
void DestroyDescriptorSet(DescriptorSetData data)
{
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga