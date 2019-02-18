#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void SetScissors(CommandBufferData data, uint32_t index, VkRect2D rect)
{
  vkCmdSetScissor(data.Buffer[index], 0, 1, &rect);
}
void SetViewport(CommandBufferData data, uint32_t index, VkViewport viewport)
{
  vkCmdSetViewport(data.Buffer[index], 0, 1, &viewport);
}
void CommandBufferDrawExample(CommandBufferData data, uint32_t index)
{
  vkCmdDraw(data.Buffer[index], 3, 1, 0, 0);
}
void BindCommandBufferPipeline(CommandBufferData data, uint32_t index, PipelineData pipeline)
{
  vkCmdBindPipeline(data.Buffer[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.Pipeline);
}
void BeginCommandBufferRenderPass(CommandBufferData data, uint32_t index, VkFramebuffer framebuffer, VkRenderPass renderPass, VkOffset2D offset, VkExtent2D extent)
{
  std::vector<VkClearValue> clearColor(1);
  {
    clearColor[0] = {0.0f, 0.0f, 0.0f, 1.0f};
  }

  auto renderPassInfo = VkRenderPassBeginInfo();
  {
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = offset;
    renderPassInfo.renderArea.extent = extent;
    renderPassInfo.clearValueCount = clearColor.size();
    renderPassInfo.pClearValues = clearColor.data();
  }
  vkCmdBeginRenderPass(data.Buffer[index], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
}
void EndCommandBufferRenderPass(CommandBufferData data, uint32_t index)
{
  vkCmdEndRenderPass(data.Buffer[index]);
}
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index, RenderPassData renderPass, uint32_t subPass)
{
  auto inheritanceInfo = VkCommandBufferInheritanceInfo();
  {
    inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritanceInfo.renderPass = renderPass.RenderPass;
    inheritanceInfo.subpass = subPass;
  }

  auto beginInfo = VkCommandBufferBeginInfo();
  {
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
  }

  if (vkBeginCommandBuffer(data.Buffer[index], &beginInfo) != VK_SUCCESS)
  {
    Console::Fatal("Failed to start recording command buffer!");
  }
}
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index)
{
  auto beginInfo = VkCommandBufferBeginInfo();
  {
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  }

  if (vkBeginCommandBuffer(data.Buffer[index], &beginInfo) != VK_SUCCESS)
  {
    Console::Fatal("Failed to start recording command buffer!");
  }
}
void EndCommandBufferRecording(CommandBufferData data, uint32_t index)
{
  vkEndCommandBuffer(data.Buffer[index]);
}
CommandBufferData CreateCommandBuffer(DeviceData device, CommandPoolData pool, VkCommandBufferLevel level, uint32_t size)
{
  auto data = CommandBufferData();

  auto allocInfo = VkCommandBufferAllocateInfo();
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool.Pool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = size;
  }

  data.Buffer.resize(size);
  if (vkAllocateCommandBuffers(device.Device, &allocInfo, data.Buffer.data()) != VK_SUCCESS)
  {
    Console::Fatal("Failed to allocate command buffers on device: {0}", Console::Arguments() << device.Properties.deviceName);
  }

  return data;
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga