#include "Renderer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VkSemaphore CreateSemaphire(DeviceData device)
{
  auto semaphoreInfo = VkSemaphoreCreateInfo();
  {
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  }
  VkSemaphore semaphore;
  if (vkCreateSemaphore(device.Device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create semaphore on device: {0}", device.Properties.deviceName);
  }
  return semaphore;
}

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
  std::vector<VkSemaphore> waitSemaphores = {data.ImageAvailableSemaphore};
  std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  std::vector<VkSemaphore> signalSemaphores = {data.RenderFinishedSemaphore};

  auto submitInfo = VkSubmitInfo();
  {
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.signalSemaphoreCount = signalSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.commandBufferCount = data.CommandBuffer.Buffer.size();
    submitInfo.pCommandBuffers = data.CommandBuffer.Buffer.data();
  }
  if (vkQueueSubmit(data.Device.GraphicQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
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

  data.CommandPool = CreateCommandPool(device);
  data.CommandBuffer = CreateCommandBuffer(device, data.CommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

  data.ImageAvailableSemaphore = CreateSemaphire(device);
  data.RenderFinishedSemaphore = CreateSemaphire(device);

  return data;
}
void DestroyRenderer(RendererData data)
{
  DestroyCommandPool(data.CommandPool);

  vkDestroySemaphore(data.Device.Device, data.ImageAvailableSemaphore, nullptr);
  vkDestroySemaphore(data.Device.Device, data.RenderFinishedSemaphore, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga