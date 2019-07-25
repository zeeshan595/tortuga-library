#ifndef _VULKAN_DESCRIPTOR_SET
#define _VULKAN_DESCRIPTOR_SET

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./DescriptorPool.hpp"
#include "./Buffer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorSets
{
struct DescriptorSets
{
  VkDevice Device;
  VkDescriptorSet set;
  VkDescriptorPool Pool;
  uint32_t DescriptorSetCount;
  std::vector<DescriptorLayout::DescriptorLayout> Layouts;
};

DescriptorSets Create(Device::Device device, DescriptorPool::DescriptorPool pool, std::vector<DescriptorLayout::DescriptorLayout> layouts);
void Destroy(DescriptorSets data);
void UpdateDescriptorSets(DescriptorSets data, uint32_t descriptorSetIndex, std::vector<Buffer::Buffer> content);
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif