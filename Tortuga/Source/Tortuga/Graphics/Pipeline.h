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
};
} // namespace Tortuga

#endif