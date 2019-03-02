#include "DescriptorLayout.h"

namespace Tortuga
{
namespace Graphics
{
DescriptorLayout CreateDescriptorLayouts(HardwareController hardware)
{
  auto data = DescriptorLayout();
  data.hardware = hardware;

  data.UniformLayouts.resize(hardware.Devices.size());
  data.ImageLayouts.resize(hardware.Devices.size());

  std::vector<VkDescriptorSetLayoutBinding> bindings(2);
  {
    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[1].binding = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.UniformLayouts[i] = VulkanAPI::CreateDescriptorSetLayout(hardware.Devices[i].VulkanDevice, {bindings[0]});
    data.ImageLayouts[i] = VulkanAPI::CreateDescriptorSetLayout(hardware.Devices[i].VulkanDevice, {bindings[1]});
  }
  return data;
}
void DestroyDescriptorLayouts(DescriptorLayout data)
{
  for (uint32_t i = 0; i < data.hardware.Devices.size(); i++)
  {
    VulkanAPI::DestroyDescriptorSetLayout(data.UniformLayouts[i]);
    VulkanAPI::DestroyDescriptorSetLayout(data.ImageLayouts[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga