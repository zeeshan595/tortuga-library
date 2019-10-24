#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

#include <vulkan/vulkan.h>
#include <string>

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
struct Shader
{
  VkDevice Device = VK_NULL_HANDLE;
  VkShaderModule Shader = VK_NULL_HANDLE;
  VkShaderStageFlagBits Type;
};

struct FullShaderCode
{
  std::string code;
  std::string location;
  std::string type;
  std::string file;
};
FullShaderCode GetFullShaderCode(const std::string file);
std::string CompileShader(const std::string fullShaderCode, const std::string location, const std::string type);
Shader Create(const Device::Device device, const std::string compiled, const VkShaderStageFlagBits type);
void Destroy(const Shader data);
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif