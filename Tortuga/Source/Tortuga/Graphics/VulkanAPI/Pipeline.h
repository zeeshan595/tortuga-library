#ifndef _VULKAN_API_PIPELINE
#define _VULKAN_API_PIPELINE

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
PipelineData CreatePipeline(DeviceData device, SwapchainData swapchain, RenderPassData renderPass, std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos);
void DestroyPipeline(PipelineData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif