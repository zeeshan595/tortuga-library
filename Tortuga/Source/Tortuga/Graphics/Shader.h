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
  Vertex,
  Fragment,
  Geometry,
  Compute
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