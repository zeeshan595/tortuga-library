#include "./Shader.hpp"

#include <regex>
#include <vector>
#include "../../Utils/IO.hpp"
#include "../../Core/GUID.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
//private
std::string GetFileType(std::string file)
{
  size_t found;
  found = file.find_last_of(".");
  return file.substr(found + 1, file.size() - found - 1);
}
std::string GetFileLocation(std::string file)
{
  size_t found;
  found = file.find_last_of("/");
  return file.substr(0, found);
}
std::string ResolveShaderIncludes(std::string location, std::string code)
{
  std::string fullShader = code;
  std::regex pattern("#include[ ]*\"([a-zA-Z0-9./]+)\"");
  std::smatch matches;
  while (std::regex_search(fullShader, matches, pattern))
  {
    if (matches.size() < 2)
      continue;
    const auto fullMatch = matches[0].str();
    const auto includeFile = matches[1].str();
    const auto includedShader = Utils::IO::GetFileContents(location + includeFile).data();
    const auto includeCode = ResolveShaderIncludes(GetFileLocation(location + includeFile), includedShader);
    const auto includeIndex = fullShader.find(fullMatch);
    fullShader.replace(includeIndex, fullMatch.length(), includeCode);
  }
  return fullShader;
}
//public
FullShaderCode GetFullShaderCode(const std::string file)
{
  const auto location = GetFileLocation(file);
  const auto fullShaderCode = ResolveShaderIncludes(location, Utils::IO::GetFileContents(file).data());
  auto data = FullShaderCode();
  data.code = fullShaderCode;
  data.file = file;
  data.location = location;
  data.type = GetFileType(file);
  return data;
}
std::string CompileShader(const std::string fullShaderCode, const std::string location, const std::string type)
{
  //compile
  const auto tempShaderPath = location + "/" + Core::GUID::GenerateGUID(1) + "." + type;
  Utils::IO::SetFileContents(tempShaderPath, fullShaderCode);
  const std::string cmd = "Build/bin/glslangValidator -s -V " + tempShaderPath + " -o " + tempShaderPath + ".spv";
  system(cmd.c_str());
  const auto compiledShader = Utils::IO::GetFileContents(tempShaderPath + ".spv");
  const std::string cleanUpCommand = "rm " + tempShaderPath + " && rm " + tempShaderPath + ".spv";
  system(cleanUpCommand.c_str());

  return compiledShader;
}
Shader Create(const Device::Device device, const std::string compiled, const VkShaderStageFlagBits type)
{
  Shader data = {};
  data.Type = type;
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
void Destroy(const Shader data)
{
  if (data.Shader == VK_NULL_HANDLE)
    return;

  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga