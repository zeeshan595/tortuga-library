#include "./Framebuffer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Framebuffer
{
Framebuffer Create(Device::Device device, uint32_t width, uint32_t height, RenderPass::RenderPass renderPass, std::vector<ImageView::ImageView> imageViews)
{
  Framebuffer data = {};
  data.Device = device.Device;

  std::vector<VkImageView> attachments(imageViews.size());
  for (uint32_t i = 0; i < imageViews.size(); i++)
    attachments[i] = imageViews[i].View;

  VkFramebufferCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = renderPass.RenderPass;
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments = attachments.data();
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = 1;
  }

  ErrorCheck::Callback(vkCreateFramebuffer(data.Device, &createInfo, nullptr, &data.Framebuffer));

  return data;
}
void Destroy(Framebuffer data)
{
  vkDestroyFramebuffer(data.Device, data.Framebuffer, nullptr);
}
} // namespace Image
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga