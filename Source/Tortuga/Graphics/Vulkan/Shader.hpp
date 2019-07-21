#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
struct Shader
{
  VkDevice Device = VK_NULL_HANDLE;
  VkShaderModule Shader = VK_NULL_HANDLE;
};

Shader Create(Device::Device device, std::vector<char> code);
void Destroy(Shader data);
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif