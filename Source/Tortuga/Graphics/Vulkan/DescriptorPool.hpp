#ifndef _VULKAN_DESCRIPTOR_POOL
#define _VULKAN_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>
#include <unordered_map>

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
};

DescriptorPool Create(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, uint32_t descriptorSetsCount = 1);
void Destroy(DescriptorPool data);
} // namespace DescriptorPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif