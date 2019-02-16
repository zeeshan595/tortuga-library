#include "Shader.h"

namespace Tortuga
{
namespace Graphics
{
Shader CreateShaderFromFile(Window window, std::string filePath, ShaderType type)
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

  return CreateShader(window, buffer, type);
}

Shader CreateShader(Window window, std::vector<char> code, ShaderType type)
{
  auto data = Shader();

  VkShaderStageFlagBits shaderFlags;
  switch (type)
  {
  case ShaderType::Vertex:
    shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
    break;
  case ShaderType::Fragment:
    shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    break;
  case ShaderType::Geometry:
    shaderFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
    break;
  case ShaderType::Compute:
    shaderFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    break;
  default:
    shaderFlags = VK_SHADER_STAGE_ALL;
    break;
  }

  data.VulkanShader.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanShader[i] = VulkanAPI::CreateShader(window.VulkanDevicesInUse[i], code, shaderFlags);
  }

  return data;
}
void DestroyShader(Shader data)
{
  for (uint32_t i = 0; i < data.VulkanShader.size(); i++)
  {
    VulkanAPI::DestroyShader(data.VulkanShader[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga