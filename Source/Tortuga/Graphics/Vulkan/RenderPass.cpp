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

  VkAttachmentDescription depthAttachment = {};
  {
    depthAttachment.format = Image::findDepthFormat(device.PhysicalDevice);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }
  data.Attachments.push_back(depthAttachment);

  VkAttachmentReference colorAttachmentRef = {};
  {
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }

  VkAttachmentReference depthAttachmentRef = {};
  {
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }

  VkSubpassDescription subpass = {};
  {
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
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

  return data;
}
void Destroy(RenderPass data)
{
  if (data.RenderPass == VK_NULL_HANDLE)
    return;

  vkDestroyRenderPass(data.Device, data.RenderPass, nullptr);
}
} // namespace RenderPass
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga