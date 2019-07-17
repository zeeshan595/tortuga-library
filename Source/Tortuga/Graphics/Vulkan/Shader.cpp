#include "./Shader.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
Shader Create(Device::Device device, std::vector<char> code)
{
  Shader data = {};
  data.Device = device.Device;

  VkShaderModuleCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  }
  ErrorCheck::Callback(vkCreateShaderModule(device.Device, &createInfo, nullptr, &data.Shader));
  return data;
}

void Destroy(Shader data)
{
  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga