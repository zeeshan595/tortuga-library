#ifndef _COMMAND_BUFFER
#define _COMMAND_BUFFER

#include "VulkanAPI/CommandBuffer.h"

#include "HardwareController.h"
#include "CommandPool.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
enum CommandBufferLevel
{
  COMMAND_BUFFER_PRIMARY,
  COMMAND_BUFFER_SECONDARY
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

void CommandBufferSubmitCommands(CommandBuffer command);
void CommandBufferUpdateBuffer(CommandBuffer command, uint32_t index, Buffer buffer);
void CommandBufferDraw(CommandBuffer command, uint32_t index, Buffer vertexBuffer, Buffer indexBuffer, uint8_t indicesSize);
void BindCommandBufferPipeline(CommandBuffer command, uint32_t index, Pipeline pipeline);
void BeginCommandBuffer(CommandBuffer command, uint32_t index, RenderPass renderPass, uint32_t subPass);
void BeginCommandBuffer(CommandBuffer command, uint32_t index);
void EndCommandBuffer(CommandBuffer command, uint32_t index);
CommandBuffer CreateCommandBuffer(HardwareController hardware, CommandPool pool, CommandBufferLevel level, uint32_t size);
}; // namespace Graphics
}; // namespace Tortuga

#endif