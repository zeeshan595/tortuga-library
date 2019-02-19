#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "VulkanAPI/CommandBuffer.h"

#include "HardwareController.h"
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
struct CommandBufferRect
{
  uint32_t OffsetX;
  uint32_t OffsetY;
  uint32_t Width;
  uint32_t Height;
};
struct CommandBuffer
{
  HardwareController Hardware;
  std::vector<VulkanAPI::CommandBufferData> CommandBuffers;
};

void CommandBufferDrawExample(CommandBuffer command, uint32_t index);
void BindCommandBufferPipeline(CommandBuffer command, uint32_t index, Pipeline pipeline);
void BeginCommandBuffer(CommandBuffer command, uint32_t index, RenderPass renderPass, uint32_t subPass);
void BeginCommandBuffer(CommandBuffer command, uint32_t index);
void EndCommandBuffer(CommandBuffer command, uint32_t index);
CommandBuffer CreateCommandBuffer(HardwareController hardware, CommandPool pool, CommandBufferLevel level, uint32_t size);
}; // namespace Graphics
}; // namespace Tortuga

#endif