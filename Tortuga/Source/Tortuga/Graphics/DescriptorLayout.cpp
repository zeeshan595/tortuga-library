#include "DescriptorLayout.h"

namespace Tortuga
{
namespace Graphics
{
DescriptorLayout CreateDescriptorLayouts(HardwareController hardware)
{
  auto data = DescriptorLayout();
  data.hardware = hardware;

  data.Layouts.resize(hardware.Devices.size());

  std::vector<VkDescriptorSetLayoutBinding> bindings(2);
  {
    bindings[0].binding = 0;
    bindings[0].descriptorCount = 2;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[1].binding = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.Layouts[i] = VulkanAPI::CreateDescriptorSetLayout(hardware.Devices[i].VulkanDevice, bindings);
  }
  return data;
}
void DestroyDescriptorLayouts(DescriptorLayout data)
{
  for (uint32_t i = 0; i < data.hardware.Devices.size(); i++)
  {
    VulkanAPI::DestroyDescriptorSetLayout(data.Layouts[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga