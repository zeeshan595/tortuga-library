#include "Framebuffer.h"

namespace Tortuga
{
Framebuffer::Framebuffer(Pipeline *pipeline, std::vector<VkImageView> images)
{
    this->_device = pipeline->GetDevice();
    auto swapchainExtent = pipeline->GetSwapchain()->GetExtent2D();

    auto frameBufferInfo = VkFramebufferCreateInfo();
    {
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = pipeline->GetRenderPass();
        frameBufferInfo.attachmentCount = images.size();
        frameBufferInfo.pAttachments = images.data();
        frameBufferInfo.width = swapchainExtent.width;
        frameBufferInfo.height = swapchainExtent.height;
        frameBufferInfo.layers = 1;
    }

    if (vkCreateFramebuffer(_device->GetVirtualDevice(), &frameBufferInfo, nullptr, &_frameBuffer) != VK_SUCCESS)
    {
        Console::Error("Failed to create frame buffer!");
    }
}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(_device->GetVirtualDevice(), _frameBuffer, nullptr);
}
}; // namespace Tortuga