#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "VulkanAPI/CommandBuffer.h"

#include "Window.h"
#include "CommandPool.h"
#include "RenderPass.h"

namespace Tortuga
{
namespace Graphics
{
enum CommandBufferLevel
{
  CommandBufferPrimary,
  CommandBufferSecondary
};
struct CommandBuffer
{
  std::vector<VulkanAPI::CommandBufferData> CommandBuffers;
};

void BeginCommandBuffer(CommandBuffer command, uint32_t index);
void BeginCommandBuffer(CommandBuffer command, uint32_t index);
void EndCommandBuffer(CommandBuffer command, uint32_t index);
CommandBuffer CreateCommandBuffer(Window window, CommandPool pool, CommandBufferLevel level, uint32_t size);
}; // namespace Graphics
}; // namespace Tortuga

#endif