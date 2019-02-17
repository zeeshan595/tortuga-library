#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{

void BeginCommandBuffer(CommandBuffer command, uint32_t index, RenderPass renderPass, uint32_t subPass)
{
  for (uint32_t i = 0; i < command.CommandBuffers.size(); i++)
  {
    VulkanAPI::BeginCommandBufferRecording(command.CommandBuffers[i], index, renderPass.VulkanRenderPass[i], subPass);
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

CommandBuffer CreateCommandBuffer(Window window, CommandPool pool, CommandBufferLevel level, uint32_t size)
{
  auto data = CommandBuffer();

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

  data.CommandBuffers.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < data.CommandBuffers.size(); i++)
  {
    data.CommandBuffers[i] = VulkanAPI::CreateCommandBuffer(
        window.VulkanDevicesInUse[i],
        pool.VulkanCommandPool[i],
        bufferLevel,
        size);
  }

  return data;
}
}; // namespace Graphics
}; // namespace Tortuga