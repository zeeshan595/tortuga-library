#include "./Shader.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Shader
{
Shader Create(Device::Device device, std::vector<char> code)
{
  Shader data = {};
  data.Device = device.Device;

  auto codeData = reinterpret_cast<const uint32_t *>(code.data());
  VkShaderModuleCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = codeData;
  }
  ErrorCheck::Callback(vkCreateShaderModule(device.Device, &createInfo, nullptr, &data.Shader));

  VkPipelineCacheCreateInfo cacheInfo = {};
  {
    cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheInfo.initialDataSize = code.size();
    cacheInfo.pInitialData = codeData;
  }
  ErrorCheck::Callback(vkCreatePipelineCache(device.Device, &cacheInfo, nullptr, &data.TempCache));

  return data;
}

void Destroy(Shader data)
{
  vkDestroyPipelineCache(data.Device, data.TempCache, nullptr);
  vkDestroyShaderModule(data.Device, data.Shader, nullptr);
}
} // namespace Shader
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga