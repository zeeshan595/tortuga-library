#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "../Core.h"
#include "../Console.h"

#include "Device.h"
#include "Pipeline.h"
#include "Framebuffer.h"

namespace Tortuga
{
class CommandBuffer
{
private:
  Pipeline *_pipeline;
  Device *_device;
  Swapchain *_swapchain;
  std::vector<Framebuffer *> _frameBuffers;
  std::vector<VkCommandBuffer> _commandBuffer;

public:
  CommandBuffer(Pipeline *pipeline, std::vector<Framebuffer *> frameBuffers);
  ~CommandBuffer();
};
}; // namespace Tortuga

#endif