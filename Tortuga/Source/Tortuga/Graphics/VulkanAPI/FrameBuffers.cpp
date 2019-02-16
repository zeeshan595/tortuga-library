#include "FrameBuffers.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
FrameBufferData CreateFrameBuffers(DeviceData device, SwapchainData swapchain, RenderPassData renderPass)
{
  auto data = FrameBufferData();
  data.Device = device.Device;

  data.FrameBuffers.resize(swapchain.ImageViews.size());
  for (uint32_t i = 0; i < swapchain.ImageViews.size(); i++)
  {
    std::vector<VkImageView> attachments = {
        swapchain.ImageViews[i]};

    auto frameBufferInfo = VkFramebufferCreateInfo();
    {
      frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      frameBufferInfo.renderPass = renderPass.RenderPass;
      frameBufferInfo.attachmentCount = attachments.size();
      frameBufferInfo.pAttachments = attachments.data();
      frameBufferInfo.width = swapchain.Extent.width;
      frameBufferInfo.height = swapchain.Extent.height;
      frameBufferInfo.layers = 1;
    }
    if (vkCreateFramebuffer(device.Device, &frameBufferInfo, nullptr, &data.FrameBuffers[i]) != VK_SUCCESS)
    {
      Console::Fatal("Failed to create framebuffer on device: {0}", Console::Arguments() << device.Properties.deviceName);
    }
  }

  return data;
}
void DestroyFrameBuffers(FrameBufferData data)
{
  for (uint32_t i = 0; i < data.FrameBuffers.size(); i++)
  {
    vkDestroyFramebuffer(data.Device, data.FrameBuffers[i], nullptr);
  }
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga