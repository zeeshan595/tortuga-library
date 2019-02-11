#ifndef _VULKAN_API_PIPELINE
#define _VULKAN_API_PIPELINE

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "Swapchain.h"
#include "Vertex.h"
#include "Shader.h"
#include "DescriptorSetLayout.h"
#include "PipelineLayout.h"
#include "RenderPass.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class Pipeline
{
private:
  Swapchain *_swapchain;
  Device *_device;
  VkPipeline _graphicsPipeline;

public:
  Pipeline(Device *device, Swapchain *swapchain, RenderPass *renderPass, Shader *shader, PipelineLayout *pipelineLayout);
  ~Pipeline();

  Swapchain *GetSwapchain() { return _swapchain; }
  Device *GetDevice() { return _device; }
  VkPipeline GetVulkanPipeline() { return _graphicsPipeline; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif