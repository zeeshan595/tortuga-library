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

  Buffer *vertexBuffer;
  Buffer *indexBuffer;

  const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

  const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0};

public:
  const uint32_t MAX_FRAMES_QUEUED = 2;

  CommandBuffer(Pipeline *pipeline, std::vector<Framebuffer *> frameBuffers);
  ~CommandBuffer();

  void SetupDrawCall();
  void Render();

  std::vector<VkCommandBuffer> GetCommandBuffers() { return _commandBuffer; }
};
}; // namespace Tortuga

#endif