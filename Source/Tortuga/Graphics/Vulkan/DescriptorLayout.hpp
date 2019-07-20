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
struct Binding
{
  uint32_t DescriptorCount = 1;
  VkDescriptorType Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  VkShaderStageFlags ShaderStage = VK_SHADER_STAGE_ALL;
  VkSampler Sampler = VK_NULL_HANDLE;
};
struct DescriptorLayout
{
  VkDevice Device = VK_NULL_HANDLE;
  VkDescriptorSetLayout Layouts = VK_NULL_HANDLE;
  std::vector<VkDescriptorPoolSize> PoolSizes;
};

DescriptorLayout Create(Device::Device device, std::vector<Binding> bindings);
void Destroy(DescriptorLayout data);
} // namespace DescriptorLayout
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif