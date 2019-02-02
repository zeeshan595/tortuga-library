#ifndef _SHADER
#define _SHADER

#include "../Core.h"
#include "../Console.h"
#include "Device.h"

namespace Tortuga
{
class Shader
{
private:
  Device *_device;
  VkShaderModule _vertexShader;
  VkShaderModule _fragmentShader;
  std::vector<VkPipelineShaderStageCreateInfo> _pipelineShaderInfo;

  static std::vector<char> ReadFile(const std::string &filename);
  static VkShaderModule CreateShader(VkDevice device, std::vector<char> code);

public:
  Shader(Device *device, std::string vertexPath, std::string fragmentPath);
  ~Shader();

  VkShaderModule GetVertexShader() { return _vertexShader; }
  VkShaderModule GetFragmentShader() { return _fragmentShader; }
  std::vector<VkPipelineShaderStageCreateInfo> GetPipelineShaderInfo() { return _pipelineShaderInfo; }
};
}; // namespace Tortuga

#endif