#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
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
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
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