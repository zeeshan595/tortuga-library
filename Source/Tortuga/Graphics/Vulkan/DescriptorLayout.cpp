#include "./DescriptorLayout.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorLayout
{
DescriptorLayout Create(Device::Device device, std::vector<Binding> bindings)
{
  DescriptorLayout data = {};
  data.Device = device.Device;

  data.PoolSizes.resize(bindings.size());
  std::vector<VkDescriptorSetLayoutBinding> pBindings(bindings.size());
  for (uint32_t i = 0; i < bindings.size(); i++)
  {
    //pool sizes
    data.PoolSizes[i].descriptorCount = bindings[i].DescriptorCount;
    data.PoolSizes[i].type = bindings[i].Type;

    //bindings
    pBindings[i].binding = i;
    pBindings[i].descriptorType = bindings[i].Type;
    pBindings[i].descriptorCount = bindings[i].DescriptorCount;
    pBindings[i].stageFlags = bindings[i].ShaderStage;
    pBindings[i].pImmutableSamplers = &bindings[i].Sampler;
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