#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.h>

#include "./ErrorCheck.hpp"
#include "./Instance.hpp"
#include "./Device.hpp"
#include "../../Utils/IO.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
enum ShaderType
{
  COMPUTE,
  VERTEX,
  FRAGMENT
};
struct Shader
{
  VkDevice Device = VK_NULL_HANDLE;
  VkShaderModule Shader = VK_NULL_HANDLE;
};

std::vector<char> CompileShader(Instance::Instance instance, ShaderType type, std::vector<char> code);
Shader Create(Device::Device device, std::vector<char> compiled);
void Destroy(Shader data);
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif