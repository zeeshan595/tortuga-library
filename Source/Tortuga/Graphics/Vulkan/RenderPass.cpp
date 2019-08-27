#include "./RenderPass.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace RenderPass
{
RenderPass Create(Device::Device device, VkFormat imageFormat)
{
  RenderPass data = {};
  data.Device = device.Device;
  data.Attachments.clear();

  VkAttachmentDescription colorAttachment = {};
  {
    colorAttachment.format = imageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  }
  data.Attachments.push_back(colorAttachment);

  VkAttachmentReference colorAttachmentRef = {};
  {
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }

  VkSubpassDescription subpass = {};
  {
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    //subpass.pDepthStencilAttachment = &depthAttachment;
  }

  VkRenderPassCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = data.Attachments.size();
    createInfo.pAttachments = data.Attachments.data();
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;
  }

  ErrorCheck::Callback(vkCreateRenderPass(data.Device, &createInfo, nullptr, &data.RenderPass));
}
void Destroy(RenderPass data)
{
  vkDestroyRenderPass(data.Device, data.RenderPass, nullptr);
}
} // namespace RenderPass
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga