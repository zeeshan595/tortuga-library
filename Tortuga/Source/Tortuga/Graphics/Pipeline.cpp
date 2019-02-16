#include "Pipeline.h"

namespace Tortuga
{
namespace Graphics
{
Pipeline CreatePipeline(WindowData window, std::vector<Shader> shaders)
{
  auto data = Pipeline();

  data.VulkanRenderPass.resize(window.VulkanDevicesInUse.size());
  data.VulkanPipeline.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    std::vector<VkPipelineShaderStageCreateInfo> shaderInfos(shaders.size());
    for (uint32_t j = 0; j < shaders.size(); j++)
    {
      shaderInfos[j] = shaders[j].VulkanShader[i].StageInfo;
    }

    data.VulkanRenderPass[i] = VulkanAPI::CreateRenderPass(window.VulkanDevicesInUse[i], window.VulkanSwapchain[i]);
    data.VulkanPipeline[i] = VulkanAPI::CreatePipeline(
        window.VulkanDevicesInUse[i],
        window.VulkanSwapchain[i],
        data.VulkanRenderPass[i],
        shaderInfos);
  }

  return data;
}
void DestroyPipeline(Pipeline data)
{
  for (uint32_t i = 0; i < data.VulkanPipeline.size(); i++)
  {
    VulkanAPI::DestroyPipeline(data.VulkanPipeline[i]);
    VulkanAPI::DestroyRenderPass(data.VulkanRenderPass[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga