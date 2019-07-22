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
};

DescriptorPool Create(Device::Device device, DescriptorLayout::DescriptorLayout layout);
void Destroy(DescriptorPool data);
} // namespace DescriptorPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif