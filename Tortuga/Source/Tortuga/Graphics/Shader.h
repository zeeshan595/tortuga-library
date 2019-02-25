#ifndef _SHADER
#define _SHADER

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/Shader.h"

#include "HardwareController.h"
#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
enum ShaderType
{
  SHADER_TYPE_VERTEX,
  SHADER_TYPE_FRAGMENT,
  SHADER_TYPE_GEOMETRY,
  SHADER_TYPE_COMPUTE
};
struct Shader
{
  std::vector<VulkanAPI::ShaderData> VulkanShader;
};

Shader CreateShaderFromFile(HardwareController hardware, std::string filePath, ShaderType type);
Shader CreateShader(HardwareController hardware, std::vector<char> code, ShaderType type);
void DestroyShader(Shader data);
}; // namespace Graphics
}; // namespace Tortuga

#endif