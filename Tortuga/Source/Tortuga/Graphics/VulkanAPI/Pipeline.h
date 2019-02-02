#ifndef _PIPELINE
#define _PIPELINE

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "RenderPass.h"
#include "Swapchain.h"
#include "Vertex.h"
#include "Shader.h"

namespace Tortuga
{
class Pipeline
{
private:
  Swapchain *_swapchain;
  Device *_device;
  VkPipelineLayout _pipelineLayout;
  VkPipeline _graphicsPipeline;

public:
  Pipeline(Device *device, Swapchain *swapchain, RenderPass *renderPass, Shader *shader);
  ~Pipeline();

  Swapchain *GetSwapchain() { return _swapchain; }
  Device *GetDevice() { return _device; }
  VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; }
  VkPipeline GetVulkanPipeline() { return _graphicsPipeline; }
};
} // namespace Tortuga

#endif