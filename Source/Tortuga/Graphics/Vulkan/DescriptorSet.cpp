#include "./DescriptorSet.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorSet
{
DescriptorSet Create(Device::Device device, DescriptorLayout::DescriptorLayout layout, DescriptorPool::DescriptorPool pool)
{
  DescriptorSet data = {};
  data.Device = device.Device;
  data.Pool = pool.Pool;
  data.DescriptorSetCount = pool.DescriptorSetCounts;

  VkDescriptorSetAllocateInfo info = {};
  {
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.descriptorPool = pool.Pool,
    info.descriptorSetCount = pool.DescriptorSetCounts;
    info.pSetLayouts = &layout.Layouts;
  }
  ErrorCheck::Callback(vkAllocateDescriptorSets(device.Device, &info, &data.set));
  return data;
}
void Destroy(DescriptorSet data)
{
  ErrorCheck::Callback(vkFreeDescriptorSets(data.Device, data.Pool, data.DescriptorSetCount, &data.set));
}
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga