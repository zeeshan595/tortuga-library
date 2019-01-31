#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "../Core.h"
#include "../Console.h"

#include "Device.h"
#include "Pipeline.h"
#include "Framebuffer.h"
#include "Buffer.h"
#include "Vertex.h"

namespace Tortuga
{
class CommandBuffer
{
private:
  size_t _currentFrame;
  Pipeline *_pipeline;
  Device *_device;
  Swapchain *_swapchain;
  std::vector<Framebuffer *> _frameBuffers;
  std::vector<VkCommandBuffer> _commandBuffer;

  std::vector<VkFence> _fences;
  std::vector<VkSemaphore> _imageAvailableSemaphore;
  std::vector<VkSemaphore> _imageFinishedSemaphore;

public:
  const uint32_t MAX_FRAMES_QUEUED = 2;

  CommandBuffer(Pipeline *pipeline, std::vector<Framebuffer *> frameBuffers);
  ~CommandBuffer();

  void SetupDrawCall(Buffer *vertexBuffer, Buffer *indexBuffer, uint32_t indicesSize, bool clear = false);
  void Render();

  std::vector<VkCommandBuffer> GetCommandBuffers() { return _commandBuffer; }
};
}; // namespace Tortuga

#endif