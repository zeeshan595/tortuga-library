#ifndef _SHADER
#define _SHADER

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/Shader.h"

#include "RenderingEngine.h"
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

Shader CreateShaderFromFile(WindowData window, std::string filePath, ShaderType type);
Shader CreateShader(WindowData window, std::vector<char> code, ShaderType type);
void DestroyShader(Shader data);
}; // namespace Graphics
}; // namespace Tortuga

#endif