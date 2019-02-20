#ifndef _RENDERER
#define _RENDERER

#include "VulkanAPI/Device.h"
#include "VulkanAPI/Renderer.h"
#include "VulkanAPI/CommandPool.h"
#include "VulkanAPI/CommandBuffer.h"

#include "HardwareController.h"
#include "FrameBuffers.h"
#include "RenderPass.h"
#include "CommandPool.h"
#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
struct Renderer
{
  Window window;
  HardwareController Hardware;
  FrameBuffer VulkanFrameBuffer;
  std::vector<VulkanAPI::RendererData> VulkanRenderers;
  VulkanAPI::CommandPoolData VulkanCommandPool;
  VulkanAPI::CommandBufferData VulkanCommandBuffer;
  VkSemaphore ImageAvaliableSemaphore;
  VkSemaphore PresentImageSemaphore;
};
void SubmitCommands(Renderer data, std::vector<CommandBuffer> commands);
void WaitForDevices(HardwareController hardware);
void DrawFrame(Renderer data);
Renderer CreateRenderer(HardwareController hardware, Window window, FrameBuffer frameBuffer, RenderPass renderPass);
void DestroyRenderer(Renderer data);
}; // namespace Graphics
}; // namespace Tortuga

#endif