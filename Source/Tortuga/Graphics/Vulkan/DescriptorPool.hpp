#ifndef _VULKAN_DESCRIPTOR_POOL
#define _VULKAN_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorPool
{
struct DescriptorPool
{
  VkDevice Device;
  VkDescriptorPool Pool;
  uint32_t DescriptorSetCounts;
  std::vector<VkDescriptorPoolSize> PoolSizes;
};

DescriptorPool Create(Device::Device device, uint32_t descriptorSetsCount = 1, std::vector<uint32_t> bindingsAmount = {1});
void Destroy(DescriptorPool data);
} // namespace DescriptorPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif