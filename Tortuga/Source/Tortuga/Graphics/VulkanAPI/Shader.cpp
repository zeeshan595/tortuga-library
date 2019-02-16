#include "Shader.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
ShaderData CreateShader(DeviceData device, std::vector<char> code)
{
  auto data = ShaderData();
  data.Device = device.Device;

  auto createInfo = VkShaderModuleCreateInfo();
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  }
  if (vkCreateShaderModule(data.Device, &createInfo, nullptr, &data.Shader) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create shader module on device: {0}", Console::Arguments() << device.Properties.deviceName);
  }

  return data;
}

ShaderData CreateShaderFromFile(DeviceData device, std::string filePath)
{
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  if (!file.is_open())
  {
    Console::Error("Failed to open file: {0}", Console::Arguments() << filePath);
    return {};
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return CreateShader(device, buffer);
}

void DestroyShader(ShaderData data)
{
  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga