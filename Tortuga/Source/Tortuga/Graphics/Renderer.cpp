#include "Renderer.h"

namespace Tortuga
{
namespace Graphics
{
void SetupPresentation(Renderer data)
{
  for (uint32_t i = 0; i < data.window.VulkanSwapchain.ImageCount; i++)
  {
    VulkanAPI::BeginCommandBufferRecording(data.VulkanCommandBuffer, i);
    VulkanAPI::CommandBufferImageLayoutTransfer(
        data.VulkanCommandBuffer, i,
        data.window.VulkanSwapchain.Images[i],
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    for (uint32_t j = 0; j < data.Hardware.Devices.size(); j++)
    {
      auto imageRegion = VkImageBlit();
      {
        VkOffset3D offset = {
            data.Hardware.Devices[j].Viewport.X,
            data.Hardware.Devices[j].Viewport.Y,
            0};
        VkOffset3D size = {
            data.Hardware.Devices[j].Viewport.Width + offset.x,
            data.Hardware.Devices[j].Viewport.Height + offset.y,
            1};

        imageRegion.srcOffsets[0] = offset;
        imageRegion.srcOffsets[1] = size;
        imageRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};

        imageRegion.dstOffsets[0] = offset;
        imageRegion.dstOffsets[1] = size;
        imageRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
      }

      vkCmdBlitImage(
          data.VulkanCommandBuffer.Buffer[i],
          data.VulkanFrameBuffer.VulkanFrameBuffers[j].Images[0].Image,
          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
          data.window.VulkanSwapchain.Images[i],
          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          1, &imageRegion, VK_FILTER_NEAREST);
    }

    VulkanAPI::CommandBufferImageLayoutTransfer(
        data.VulkanCommandBuffer, i,
        data.window.VulkanSwapchain.Images[i],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    VulkanAPI::EndCommandBufferRecording(data.VulkanCommandBuffer, i);
  }
}
void WaitForDevices(HardwareController hardware)
{
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
    vkDeviceWaitIdle(hardware.Devices[i].VulkanDevice.Device);
}
void SubmitCommands(Renderer data, std::vector<CommandBuffer> commands)
{
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    std::vector<VulkanAPI::CommandBufferData> vulkanCommands(commands.size());
    for (uint32_t j = 0; j < vulkanCommands.size(); j++)
      vulkanCommands[j] = commands[j].CommandBuffers[i];

    VulkanAPI::SubmitCommands(data.VulkanRenderers[i], vulkanCommands);
  }
}
void DrawFrame(Renderer data)
{
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
    VulkanAPI::DrawFrame(data.VulkanRenderers[i]);

  for (uint32_t i = 0; i < data.Hardware.Devices.size(); i++)
    vkDeviceWaitIdle(data.Hardware.Devices[i].VulkanDevice.Device);

  uint32_t imageIndex;
  if (vkAcquireNextImageKHR(
          data.Hardware.VulkanMainDevice.Device,
          data.window.VulkanSwapchain.Swapchain,
          std::numeric_limits<uint64_t>::max(),
          data.ImageAvaliableSemaphore,
          VK_NULL_HANDLE,
          &imageIndex) != VK_SUCCESS)
  {
    Console::Fatal("'AcquireNextImageKHR' failed on device: {0}", data.Hardware.VulkanMainDevice.Properties.deviceName);
  }

  std::vector<VkSemaphore> imageAvaliable = {data.ImageAvaliableSemaphore};
  std::vector<VkSemaphore> presentImage = {data.PresentImageSemaphore};
  std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  //Combine multiple devices image into a single image (Blit Image)
  auto queueInfo = VkSubmitInfo();
  {
    queueInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    queueInfo.commandBufferCount = data.VulkanCommandBuffer.Buffer.size();
    queueInfo.pCommandBuffers = data.VulkanCommandBuffer.Buffer.data();
    queueInfo.waitSemaphoreCount = 0;
    queueInfo.pWaitSemaphores = nullptr;
    queueInfo.signalSemaphoreCount = presentImage.size();
    queueInfo.pSignalSemaphores = presentImage.data();
    queueInfo.pWaitDstStageMask = waitStages.data();
  }
  if (vkQueueSubmit(data.Hardware.VulkanMainDevice.GraphicQueue, 1, &queueInfo, VK_NULL_HANDLE) != VK_SUCCESS)
  {
    Console::Fatal("Failed to submit command to device: {0}", data.Hardware.VulkanMainDevice.Properties.deviceName);
  }

  //Present image on the screen
  std::vector<VkSwapchainKHR> swapChains = {data.window.VulkanSwapchain.Swapchain};
  auto presentInfo = VkPresentInfoKHR();
  {
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pWaitSemaphores = nullptr;
    presentInfo.swapchainCount = swapChains.size();
    presentInfo.pSwapchains = swapChains.data();
    presentInfo.pImageIndices = &imageIndex;
  }
  if (vkQueuePresentKHR(data.Hardware.VulkanMainDevice.PresentQueue, &presentInfo) != VK_SUCCESS)
  {
    Console::Fatal("Failed to present image from device: {0}", data.Hardware.VulkanMainDevice.Properties.deviceName);
  }
  WaitForDevices(data.Hardware);
}
Renderer CreateRenderer(HardwareController hardware, Window window, FrameBuffer frameBuffer, RenderPass renderPass)
{
  auto data = Renderer();
  data.VulkanFrameBuffer = frameBuffer;
  data.window = window;
  data.Hardware = hardware;

  data.VulkanRenderers.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanRenderers[i] = VulkanAPI::CreateRenderer(
        hardware.Devices[i].VulkanDevice,
        frameBuffer.VulkanFrameBuffers[i],
        renderPass.VulkanRenderPass[i]);
  }

  auto semaphoreInfo = VkSemaphoreCreateInfo();
  {
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  }

  if (vkCreateSemaphore(data.Hardware.VulkanMainDevice.Device, &semaphoreInfo, nullptr, &data.PresentImageSemaphore) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create semaphore for presentation on device: {0}", data.Hardware.VulkanMainDevice.Properties.deviceName);
  }
  if (vkCreateSemaphore(data.Hardware.VulkanMainDevice.Device, &semaphoreInfo, nullptr, &data.ImageAvaliableSemaphore) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create semaphore for presentation on device: {0}", data.Hardware.VulkanMainDevice.Properties.deviceName);
  }

  data.VulkanCommandPool = VulkanAPI::CreateCommandPool(
      hardware.VulkanMainDevice);
  data.VulkanCommandBuffer = VulkanAPI::CreateCommandBuffer(
      hardware.VulkanMainDevice,
      data.VulkanCommandPool,
      VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      window.VulkanSwapchain.ImageCount);
  SetupPresentation(data);
  return data;
}
void DestroyRenderer(Renderer data)
{
  WaitForDevices(data.Hardware);
  VulkanAPI::DestroyCommandPool(data.VulkanCommandPool);
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    VulkanAPI::DestroyRenderer(data.VulkanRenderers[i]);
  }
  vkDestroySemaphore(data.Hardware.VulkanMainDevice.Device, data.PresentImageSemaphore, nullptr);
  vkDestroySemaphore(data.Hardware.VulkanMainDevice.Device, data.ImageAvaliableSemaphore, nullptr);
}
}; // namespace Graphics
}; // namespace Tortuga
