#include "RenderPass.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
RenderPassData CreateRenderPass(DeviceData device, SwapchainData swapchain)
{
  auto data = RenderPassData();
  data.Device = device.Device;

  auto colorAttachment = VkAttachmentDescription();
  {
    colorAttachment.format = swapchain.SurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  }

  auto colorAttachmentRef = VkAttachmentReference();
  {
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  }

  auto subpass = VkSubpassDescription();
  {
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
  }

  auto dependency = VkSubpassDependency();
  {
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }

  auto renderPassInfo = VkRenderPassCreateInfo();
  {
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
  }
  if (vkCreateRenderPass(device.Device, &renderPassInfo, nullptr, &data.RenderPass) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create render pass on device: {0}", Console::Arguments() << device.Properties.deviceName);
  }

  return data;
}
void DestroyRenderPass(RenderPassData data)
{
  vkDestroyRenderPass(data.Device, data.RenderPass, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga