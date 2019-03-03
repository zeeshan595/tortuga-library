#include "DescriptorSet.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void ConfigureDescriptorSetSampledImage(DescriptorSetData data, VkImageLayout imageLayout, VkImageView imageView, VkSampler imageSampler, uint32_t setIndex, uint32_t binding)
{
  auto imageInfo = VkDescriptorImageInfo();
  {
    imageInfo.imageLayout = imageLayout;
    imageInfo.imageView = imageView;
    imageInfo.sampler = imageSampler;
  }

  auto descriptorWrite = VkWriteDescriptorSet();
  {
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = data.DescriptorSet[setIndex];
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = data.DescriptorSize;
    descriptorWrite.pImageInfo = &imageInfo;
  }
  vkUpdateDescriptorSets(data.Device, 1, &descriptorWrite, 0, nullptr);
}
void ConfigureDescriptorSetBuffer(DescriptorSetData data, BufferData buffer, uint32_t setIndex, uint32_t binding)
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
    descriptorWrite.dstSet = data.DescriptorSet[setIndex];
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = data.DescriptorSize;
    descriptorWrite.pBufferInfo = &bufferInfo;
  }
  vkUpdateDescriptorSets(data.Device, 1, &descriptorWrite, 0, nullptr);
}
DescriptorSetData ConfigureDescriptorSets(DeviceData device, DescriptorLayoutData layout, DescriptorPoolData pool)
{
  auto data = DescriptorSetData();
  data.Device = device.Device;
  data.DescriptorSize = pool.DescriptorSize;

  std::vector<VkDescriptorSetLayout> layouts(pool.DescriptorSetSize, layout.Layout);
  auto allocInfo = VkDescriptorSetAllocateInfo();
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool.Pool;
    allocInfo.descriptorSetCount = pool.DescriptorSetSize;
    allocInfo.pSetLayouts = layouts.data();
  }
  data.DescriptorSet.resize(pool.DescriptorSetSize);
  VkResult temp = vkAllocateDescriptorSets(data.Device, &allocInfo, data.DescriptorSet.data());
  if (temp != VK_SUCCESS)
  {
    Console::Fatal("Failed to allocate descriptor sets on device: {0}", device.Properties.deviceName);
  }
  return data;
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga