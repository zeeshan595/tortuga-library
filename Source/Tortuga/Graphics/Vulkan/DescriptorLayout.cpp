#include "./DescriptorLayout.hpp"

/*
struct Descriptor { type specific data }

struct DescriptorBinding {   
  int binding;
  DescriptorType type;
  Descriptor descriptors[]
};

struct DescriptorSet {
    DescriptorBinding bindings[];
};

struct PipelineLayout {
  DescriptorSet sets[]
}
*/

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorLayout
{
DescriptorLayout Create(Device::Device device, uint32_t bindingsAmount, VkDescriptorType type)
{
  DescriptorLayout data = {};
  data.Device = device.Device;
  data.BindingsAmount = bindingsAmount;
  data.Type = type;

  std::vector<VkDescriptorSetLayoutBinding> pBindings(bindingsAmount);
  for (uint32_t i = 0; i < bindingsAmount; i++)
  {
    //bindings
    pBindings[i].binding = i;
    pBindings[i].descriptorType = type;
    pBindings[i].descriptorCount = 1;
    pBindings[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pBindings[i].pImmutableSamplers = VK_NULL_HANDLE;
  }
  VkDescriptorSetLayoutCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = pBindings.size();
    createInfo.pBindings = pBindings.data();
  }
  ErrorCheck::Callback(vkCreateDescriptorSetLayout(device.Device, &createInfo, nullptr, &data.Layouts));
  return data;
}
void Destroy(DescriptorLayout data)
{
  vkDestroyDescriptorSetLayout(data.Device, data.Layouts, nullptr);
}
} // namespace DescriptorLayout
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga