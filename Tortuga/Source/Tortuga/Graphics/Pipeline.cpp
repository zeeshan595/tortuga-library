#include "Pipeline.h"

namespace Tortuga
{
namespace Graphics
{
VkVertexInputBindingDescription GetBindingDescription()
{
  auto bindingDescription = VkVertexInputBindingDescription();
  {
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(glm::vec3);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  }
  return bindingDescription;
}
std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
{
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
  {
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = 0;
  }
  return attributeDescriptions;
}

Pipeline CreatePipeline(HardwareController hardware, RenderPass renderPass, std::vector<Shader> shaders)
{
  auto data = Pipeline();

  data.Layout = CreateDescriptorLayouts(hardware);
  data.VulkanPipeline.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    std::vector<VkPipelineShaderStageCreateInfo> shaderInfos(shaders.size());
    for (uint32_t j = 0; j < shaders.size(); j++)
    {
      shaderInfos[j] = shaders[j].VulkanShader[i].StageInfo;
    }

    data.VulkanPipeline[i] = VulkanAPI::CreatePipeline(
        hardware.Devices[i].VulkanDevice,
        renderPass.VulkanRenderPass[i],
        {hardware.FullWidth, hardware.FullHeight},
        shaderInfos,
        GetBindingDescription(), GetAttributeDescriptions(),
        {data.Layout.Layouts[i].Layout});
  }

  return data;
} // namespace Graphics
void DestroyPipeline(Pipeline data)
{
  for (uint32_t i = 0; i < data.VulkanPipeline.size(); i++)
  {
    VulkanAPI::DestroyPipeline(data.VulkanPipeline[i]);
  }
  DestroyDescriptorLayouts(data.Layout);
}
}; // namespace Graphics
}; // namespace Tortuga