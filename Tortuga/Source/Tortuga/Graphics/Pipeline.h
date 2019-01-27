#ifndef _PIPELINE
#define _PIPELINE

#include "../Core.h"
#include "../Console.h"
#include "Swapchain.h"

namespace Tortuga
{
class Pipeline
{
private:
  Swapchain *_swapchain;
  Device *_device;
  VkShaderModule _vertexShader;
  VkShaderModule _fragmentShader;
  VkPipelineLayout _pipelineLayout;
  VkRenderPass _renderPass;
  VkPipeline _graphicsPipeline;

  static std::vector<char> ReadFile(const std::string &filename);
  static VkShaderModule CreateShader(VkDevice device, std::vector<char> code);

public:
  Pipeline(Swapchain *swapchain, std::string vertexPath, std::string fragmentPath);
  ~Pipeline();

  Swapchain *GetSwapchain() { return _swapchain; }
  Device *GetDevice() { return _device; }
  VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; }
  VkRenderPass GetRenderPass() { return _renderPass; }
  VkPipeline GetVulkanPipeline() { return _graphicsPipeline; }
};
} // namespace Tortuga

#endif