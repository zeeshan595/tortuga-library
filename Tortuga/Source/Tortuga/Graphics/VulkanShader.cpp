#include "./VulkanShader.h"

namespace Tortuga {
namespace Graphics {
VulkanShader CreateVulkanShader(VulkanDevice device,
                                std::vector<char> shaderCode) {
  auto data = VulkanShader();
  data.VirtualDevice = device.VirtualDevice;

  auto deviceInfo = VkShaderModuleCreateInfo();
  {
    deviceInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    deviceInfo.pNext = 0;
    deviceInfo.flags = 0;
    deviceInfo.codeSize = shaderCode.size();
    deviceInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());
  }
  ErrorCheck(vkCreateShaderModule(device.VirtualDevice, &deviceInfo, nullptr,
                                  &data.ShaderModule));

  return data;
}

void DestroyVulkanShader(VulkanShader data) {
  vkDestroyShaderModule(data.VirtualDevice, data.ShaderModule, nullptr);
}
} // namespace Graphics
} // namespace Tortuga