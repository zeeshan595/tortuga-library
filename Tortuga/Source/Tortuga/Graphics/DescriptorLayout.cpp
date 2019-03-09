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

  std::vector<VkDescriptorSetLayoutBinding> uniformBindings(1);
  {
    uniformBindings[0].binding = 0;
    uniformBindings[0].descriptorCount = 1;
    uniformBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniformBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  }

  std::vector<VkDescriptorSetLayoutBinding> imageBindings(1);
  {
    imageBindings[0].binding = 0;
    imageBindings[0].descriptorCount = 1;
    imageBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    imageBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.UniformLayouts[i] = VulkanAPI::CreateDescriptorSetLayout(hardware.Devices[i].VulkanDevice, uniformBindings);
    data.ImageLayouts[i] = VulkanAPI::CreateDescriptorSetLayout(hardware.Devices[i].VulkanDevice, imageBindings);
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