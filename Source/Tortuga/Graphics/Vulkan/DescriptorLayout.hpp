#ifndef _VULKAN_DESCRIPTOR_TYPE
#define _VULKAN_DESCRIPTOR_TYPE

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace DescriptorLayout
{
struct DescriptorLayout
{
  VkDevice Device = VK_NULL_HANDLE;
  VkDescriptorSetLayout Layouts = VK_NULL_HANDLE;
  uint32_t BindingsAmount = 0;
  VkDescriptorType Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
};

DescriptorLayout Create(Device::Device device, uint32_t bindingsAmount, VkShaderStageFlags shaderStage, VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
void Destroy(DescriptorLayout data);
} // namespace DescriptorLayout
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif