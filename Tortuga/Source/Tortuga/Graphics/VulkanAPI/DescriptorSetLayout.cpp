#include "DescriptorSetLayout.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorLayoutData CreateDescriptorSetLayout(DeviceData device, std::vector<VkDescriptorSetLayoutBinding> bindingInfos)
{
  auto data = DescriptorLayoutData();
  data.Device = device.Device;

  auto layoutInfo = VkDescriptorSetLayoutCreateInfo();
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindingInfos.size();
    layoutInfo.pBindings = bindingInfos.data();
  }
  if (vkCreateDescriptorSetLayout(device.Device, &layoutInfo, nullptr, &data.Layout) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create descriptor set layout on device: {0}", device.Properties.deviceName);
  }

  return data;
}
void DestroyDescriptorSetLayout(DescriptorLayoutData data)
{
  vkDestroyDescriptorSetLayout(data.Device, data.Layout, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga