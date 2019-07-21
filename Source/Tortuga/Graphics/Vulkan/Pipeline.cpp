#include "./Pipeline.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Pipeline
{
std::vector<VkDescriptorSetLayout> GetVulkanDescriptorLayouts(std::vector<DescriptorLayout::DescriptorLayout> layouts)
{
  std::vector<VkDescriptorSetLayout> setLayouts(layouts.size());
  for (uint32_t i = 0; i < setLayouts.size(); i++)
    setLayouts[i] = layouts[i].Layouts;
  return setLayouts;
}

Pipeline CreateComputePipeline(
    Device::Device device,
    std::vector<DescriptorLayout::DescriptorLayout> layouts,
    Shader::Shader shader,
    std::vector<char> cache)
{
  Pipeline data = {};
  data.Device = device.Device;

  auto setLayouts = GetVulkanDescriptorLayouts(layouts);
  VkPipelineLayoutCreateInfo layoutInfo = {};
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();
  }
  ErrorCheck::Callback(vkCreatePipelineLayout(device.Device, &layoutInfo, nullptr, &data.Layout));

  VkPipelineCacheCreateInfo cacheInfo = {};
  {
    cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheInfo.initialDataSize = cache.size();
    cacheInfo.pInitialData = cache.data();
  }
  ErrorCheck::Callback(vkCreatePipelineCache(device.Device, &cacheInfo, nullptr, &data.Cache));

  VkComputePipelineCreateInfo pipelineInfo = {};
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    {
      pipelineInfo.stage = {};
      pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
      pipelineInfo.stage.module = shader.Shader;
      pipelineInfo.stage.pName = "main";
    }
    pipelineInfo.layout = data.Layout;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.basePipelineHandle = 0;
  }
  ErrorCheck::Callback(vkCreateComputePipelines(device.Device, data.Cache, 1, &pipelineInfo, nullptr, &data.Pipeline));
  return data;
}
void DestroyPipeline(Pipeline data)
{
  vkDestroyPipeline(data.Device, data.Pipeline, nullptr);
  vkDestroyPipelineLayout(data.Device, data.Layout, nullptr);
  vkDestroyPipelineCache(data.Device, data.Cache, nullptr);
}
std::vector<char> GetCacheData(Pipeline data)
{
  size_t cacheCount;
  ErrorCheck::Callback(vkGetPipelineCacheData(data.Device, data.Cache, &cacheCount, nullptr));
  std::vector<char> cacheData(cacheCount);
  ErrorCheck::Callback(vkGetPipelineCacheData(data.Device, data.Cache, &cacheCount, cacheData.data()));
  return cacheData;
}
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga