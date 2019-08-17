#include "./DescriptorSets.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorSets
{
DescriptorSets Create(Device::Device device, DescriptorPool::DescriptorPool pool, DescriptorLayout::DescriptorLayout layout)
{
  DescriptorSets data = {};
  data.Device = device.Device;
  data.Pool = pool;
  data.Layout = layout;

  VkDescriptorSetAllocateInfo info = {};
  {
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.descriptorPool = pool.Pool,
    info.descriptorSetCount = 1;
    info.pSetLayouts = &layout.Layouts;
  }
  ErrorCheck::Callback(vkAllocateDescriptorSets(device.Device, &info, &data.set));
  return data;
}
void UpdateDescriptorSets(DescriptorSets data, std::vector<Buffer::Buffer> content)
{
  if (data.Layout.BindingsAmount != content.size())
  {
    Console::Error("Provided Content does not match this descriptor set size");
    return;
  }

  std::vector<VkDescriptorBufferInfo> bufferInfos(content.size());
  std::vector<VkWriteDescriptorSet> writeInfos(data.Layout.BindingsAmount);
  for (uint32_t i = 0; i < data.Layout.BindingsAmount; i++)
  {
    {
      bufferInfos[i] = {};
      bufferInfos[i].buffer = content[i].Buffer;
      bufferInfos[i].offset = 0;
      bufferInfos[i].range = content[i].Size;
    }

    writeInfos[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeInfos[i].dstSet = data.set;
    writeInfos[i].dstBinding = i;
    writeInfos[i].dstArrayElement = 0;
    writeInfos[i].descriptorCount = 1;
    writeInfos[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

    writeInfos[i].pBufferInfo = &(bufferInfos[i]);
    writeInfos[i].pImageInfo = VK_NULL_HANDLE;
    writeInfos[i].pTexelBufferView = VK_NULL_HANDLE;
  }
  vkUpdateDescriptorSets(data.Device, writeInfos.size(), writeInfos.data(), 0, 0);
}
} // namespace DescriptorSets
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga