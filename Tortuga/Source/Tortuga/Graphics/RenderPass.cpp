#include "RenderPass.h"

namespace Tortuga
{
RenderPass::RenderPass(Device *device, Swapchain *swapchain)
{
    this->_device = device;

    auto colorAttachment = VkAttachmentDescription();
    {
        colorAttachment.format = swapchain->GetSurfaceFormat().format;
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

    //Render pass
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

    if (vkCreateRenderPass(_device->GetVirtualDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create render pass for vulkan pipeline");
        return;
    }
}

RenderPass::~RenderPass()
{
    vkDestroyRenderPass(_device->GetVirtualDevice(), _renderPass, nullptr);
}
};