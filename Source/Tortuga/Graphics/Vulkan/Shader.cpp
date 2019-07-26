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
  auto options = shaderc_compile_options_initialize();
  shaderc_compile_options_set_include_callbacks(
      options,
      [](void *user_data, const char *requested_source, int type, const char *requesting_source, size_t include_depth) {
        const auto requested = std::string(requested_source);
        const auto requestedName = requested.substr(requested.find_last_of('/') + 1);
        const auto contentTemp = Utils::IO::GetFileContents(std::string("Shaders/") + requested);

        uint32_t sourceCountentSize = sizeof(char) * contentTemp.size();
        const auto sourceContent = malloc(sourceCountentSize);
        memcpy(sourceContent, contentTemp.data(), sourceCountentSize);
        shaderc_include_result *result = new shaderc_include_result();
        {
          result->content = reinterpret_cast<const char *>(sourceContent);
          result->content_length = contentTemp.size();
          result->source_name = requestedName.data();
          result->source_name_length = requestedName.size();
          result->user_data = sourceContent;
        }
        return result;
      },
      [](void *user_data, shaderc_include_result *include_result) {
        free(include_result->user_data);
        delete include_result;
      },
      nullptr);
  auto result = shaderc_compile_into_spv(
      instance.ShaderCompiler,
      code.data(), code.size(),
      kind,
      filename.c_str(),
      "main", options);
  auto status = shaderc_result_get_compilation_status(result);
  if (status != shaderc_compilation_status_success)
  {
    Console::Error("Failed to compile shader: \n{0}", shaderc_result_get_error_message(result));
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