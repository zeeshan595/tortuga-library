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
  std::vector<VkDescriptorPoolSize> PoolSizes;
};

DescriptorSets Create(Device::Device device, DescriptorLayout::DescriptorLayout layout, DescriptorPool::DescriptorPool pool);
void Destroy(DescriptorSets data);
void UpdateDescriptorSet(DescriptorSets data, uint32_t descriptorSetIndex, std::vector<Buffer::Buffer> content);
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif