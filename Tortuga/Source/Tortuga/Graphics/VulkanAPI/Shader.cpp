#include "Shader.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
ShaderData CreateShader(DeviceData device, std::vector<char> code, VkShaderStageFlagBits options)
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
  auto stageInfo = VkPipelineShaderStageCreateInfo();
  {
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage = options;
    stageInfo.module = data.Shader;
    stageInfo.pName = "main";
  }
  data.StageInfo = stageInfo;

  return data;
}

ShaderData CreateShaderFromFile(DeviceData device, std::string filePath, VkShaderStageFlagBits options)
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

  return CreateShader(device, buffer, options);
}

void DestroyShader(ShaderData data)
{
  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga