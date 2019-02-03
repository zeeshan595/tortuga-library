#ifndef _RENDERER
#define _RENDERER

#include "../../Core.h"
#include "../../Console.h"

#include "Device.h"
#include "Swapchain.h"
#include "FrameBuffer.h"
#include "Buffer.h"
#include "Vertex.h"
#include "RenderPass.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "Fence.h"
#include "Semaphore.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class Renderer
{
private:
  uint32_t _frameBuffersSize;
  size_t _currentFrame;
  Pipeline *_pipeline;
  Device *_device;
  Swapchain *_swapchain;
  RenderPass *_renderPass;
  FrameBuffer *_frameBuffer;

  CommandPool *_commandPool;
  CommandBuffer *_commandBuffers;

  std::vector<Fence *> _fences;
  std::vector<Semaphore *> _imageAvailableSemaphore;
  std::vector<Semaphore *> _imageFinishedSemaphore;

public:
  const uint32_t MAX_FRAMES_QUEUED = 2;

  Renderer(Device *device, Swapchain *swapchain, RenderPass *renderPass, FrameBuffer *frameBuffer);
  ~Renderer();

  void RecordCommandBuffers(std::vector<CommandBuffer *> secondaryBuffers);
  void RenderFrame();

  void WaitForGPUIdle();
}; // namespace VulkanAPI
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif