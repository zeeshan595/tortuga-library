#include "Renderer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void SubmitCommands(RendererData data, std::vector<CommandBufferData> commandBuffers)
{
  for (uint32_t i = 0; i < data.CommandBuffer.Buffer.size(); i++)
  {
    BeginCommandBufferRecording(data.CommandBuffer, i);
    BeginCommandBufferRenderPass(
        data.CommandBuffer, i,
        data.FrameBuffers.FrameBuffers,
        data.RenderPass.RenderPass,
        {0, 0},
        {data.FrameBuffers.Width, data.FrameBuffers.Height});

    for (uint32_t j = 0; j < commandBuffers.size(); j++)
    {
      vkCmdExecuteCommands(data.CommandBuffer.Buffer[i], commandBuffers[j].Buffer.size(), commandBuffers[j].Buffer.data());
    }
    EndCommandBufferRenderPass(data.CommandBuffer, i);
    EndCommandBufferRecording(data.CommandBuffer, i);
  }
}

void DrawFrame(RendererData data)
{

  auto submitInfo = VkSubmitInfo();
  {
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = data.WaitStages.data();
    submitInfo.commandBufferCount = data.CommandBuffer.Buffer.size();
    submitInfo.pCommandBuffers = data.CommandBuffer.Buffer.data();
  }
  if (vkQueueSubmit(data.Device.GraphicQueue, 1, &submitInfo, data.OnCompleteFence) != VK_SUCCESS)
  {
    Console::Fatal("Failed to submit draw command buffer on device: {0}", Console::Arguments() << data.Device.Properties.deviceName);
  }
}

RendererData CreateRenderer(DeviceData device, FrameBufferData frameBuffers, RenderPassData renderPass)
{
  auto data = RendererData();
  data.FrameBuffers = frameBuffers;
  data.RenderPass = renderPass;
  data.Device = device;
  data.WaitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  data.CommandPool = CreateCommandPool(device);
  data.CommandBuffer = CreateCommandBuffer(device, data.CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

  auto fenceInfo = VkFenceCreateInfo();
  {
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  }
  if (vkCreateFence(device.Device, &fenceInfo, nullptr, &data.OnCompleteFence) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create a fence for renderer on device: {0}", device.Properties.deviceName);
  }

  return data;
}
void DestroyRenderer(RendererData data)
{
  DestroyCommandPool(data.CommandPool);
  vkDestroyFence(data.Device.Device, data.OnCompleteFence, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga