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
namespace DescriptorSet
{
struct DescriptorSet
{
  VkDevice Device = VK_NULL_HANDLE;
  VkDescriptorSet set = VK_NULL_HANDLE;
  DescriptorPool::DescriptorPool Pool;
  DescriptorLayout::DescriptorLayout Layout;
};

DescriptorSet Create(Device::Device device, DescriptorPool::DescriptorPool pool, DescriptorLayout::DescriptorLayout layout);
void UpdateDescriptorSets(DescriptorSet data, std::vector<Buffer::Buffer> content);
} // namespace DescriptorSet
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif