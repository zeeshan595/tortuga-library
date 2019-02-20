#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
void CommandBufferDrawExample(CommandBuffer command, uint32_t index)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::CommandBufferDrawExample(command.CommandBuffers[i], index);
  }
}
void BindCommandBufferPipeline(CommandBuffer command, uint32_t index, Pipeline pipeline)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::BindCommandBufferPipeline(command.CommandBuffers[i], index, pipeline.VulkanPipeline[i]);
  }
}
void BeginCommandBuffer(CommandBuffer command, uint32_t index, RenderPass renderPass, uint32_t subPass)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::BeginCommandBufferRecording(command.CommandBuffers[i], index, renderPass.VulkanRenderPass[i], subPass);
    auto viewport = VkViewport();
    {
      viewport.x = 0;
      viewport.y = 0;
      viewport.width = command.Hardware.FullWidth;
      viewport.height = command.Hardware.FullHeight;
      viewport.minDepth = 0;
      viewport.maxDepth = 1;
    }
    VulkanAPI::SetViewport(command.CommandBuffers[i], index, viewport);

    auto scissor = VkRect2D();
    {
      scissor.offset.x = command.Hardware.Devices[i].Viewport.X;
      scissor.offset.y = command.Hardware.Devices[i].Viewport.Y;
      scissor.extent.width = command.Hardware.Devices[i].Viewport.Width;
      scissor.extent.height = command.Hardware.Devices[i].Viewport.Height;
    }
    VulkanAPI::SetScissors(command.CommandBuffers[i], index, scissor);
  }
}
void BeginCommandBuffer(CommandBuffer command, uint32_t index)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::BeginCommandBufferRecording(command.CommandBuffers[i], index);
  }
}
void EndCommandBuffer(CommandBuffer command, uint32_t index)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::EndCommandBufferRecording(command.CommandBuffers[i], index);
  }
}

CommandBuffer CreateCommandBuffer(HardwareController hardware, CommandPool pool, CommandBufferLevel level, uint32_t size)
{
  auto data = CommandBuffer();
  data.Hardware = hardware;

  VkCommandBufferLevel bufferLevel;
  switch (level)
  {
  case CommandBufferLevel::CommandBufferPrimary:
    bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    break;
  default:
  case CommandBufferLevel::CommandBufferSecondary:
    bufferLevel = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    break;
  }

  data.CommandBuffers.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < data.CommandBuffers.size(); i++)
  {
    data.CommandBuffers[i] = VulkanAPI::CreateCommandBuffer(
        hardware.Devices[i].VulkanDevice,
        pool.VulkanCommandPool[i],
        bufferLevel,
        size);
  }

  return data;
}
}; // namespace Graphics
}; // namespace Tortuga