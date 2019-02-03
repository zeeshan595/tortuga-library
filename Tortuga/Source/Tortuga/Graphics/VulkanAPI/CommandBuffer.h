#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "CommandPool.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "Buffer.h"
#include "Swapchain.h"
#include "PipelineLayout.h"
#include "DescriptorSet.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class CommandBuffer
{
private:
  bool _isPrimary;
  Device *_device;
  CommandPool *_commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;

public:
  CommandBuffer(Device *device, CommandPool *commandPool, uint32_t amount, bool isPrimary);
  ~CommandBuffer();

  void CreateDrawCommand(uint32_t index, Buffer *vertexBuffer, Buffer *indexBuffer, uint32_t indicesSize);
  void BeginCommandBuffer(uint32_t index);
  void BeginCommandBuffer(uint32_t index, RenderPass *renderPass, uint32_t subPass);
  void EndCommandBuffer(uint32_t index);
  void BeginRenderPass(uint32_t index, Swapchain *swapchain, RenderPass *renderPass, VkFramebuffer framebuffer, VkSubpassContents subPassFlags);
  void EndRenderPass(uint32_t index);
  void BindPipeline(uint32_t index, Pipeline *pipeline);
  void BindDescriptorSet(uint32_t index, PipelineLayout *pipelineLayout, VkDescriptorSet descriptorSet);
  void SetViewport(uint32_t index, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

  std::vector<VkCommandBuffer> &GetCommandBuffers() { return _commandBuffers; }
  bool IsPrimary() { return _isPrimary; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif