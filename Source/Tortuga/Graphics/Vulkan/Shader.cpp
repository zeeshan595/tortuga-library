#include "./Shader.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
std::vector<char> CompileShader(Instance::Instance instance, ShaderType type, std::vector<char> code)
{
  shaderc_shader_kind kind;
  std::string filename;
  if (type == COMPUTE)
  {
    kind = shaderc_compute_shader;
    filename = "shader.comp";
  }
  else if (type == VERTEX)
  {
    kind = shaderc_vertex_shader;
    filename = "shader.vert";
  }
  else if (type == FRAGMENT)
  {
    kind = shaderc_fragment_shader;
    filename = "shader.frag";
  }
  else
  {
    Console::Error("Failed to compile shader, this type of shader is not supported");
    return {};
  }

  auto result = shaderc_compile_into_spv(
      instance.ShaderCompiler,
      code.data(), code.size(),
      kind,
      filename.c_str(),
      "main", nullptr);
  auto status = shaderc_result_get_compilation_status(result);
  if (status != shaderc_compilation_status_success)
  {
    Console::Error("failed to compile shader");
    return {};
  }
  auto compiled = shaderc_result_get_bytes(result);
  auto length = shaderc_result_get_length(result);
  return std::vector<char>(compiled, compiled + length);
}
Shader Create(Device::Device device, std::vector<char> compiled)
{
  Shader data = {};
  data.Device = device.Device;

  VkShaderModuleCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = compiled.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(compiled.data());
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