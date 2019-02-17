#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "VulkanAPI/CommandBuffer.h"

#include "Window.h"
#include "CommandPool.h"
#include "RenderPass.h"
#include "Pipeline.h"

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
  uint32_t WindowWidth;
  uint32_t WindowHeight;
  std::vector<uint32_t> DevicesViewportSize;
  std::vector<VulkanAPI::CommandBufferData> CommandBuffers;
};

void CommandBufferDrawExample(CommandBuffer command, uint32_t index);
void BindCommandBufferPipeline(CommandBuffer command, uint32_t index, Pipeline pipeline);
void BeginCommandBuffer(CommandBuffer command, uint32_t index, RenderPass renderPass, uint32_t subPass);
void BeginCommandBuffer(CommandBuffer command, uint32_t index);
void EndCommandBuffer(CommandBuffer command, uint32_t index);
CommandBuffer CreateCommandBuffer(Window window, CommandPool pool, CommandBufferLevel level, uint32_t size);
}; // namespace Graphics
}; // namespace Tortuga

#endif