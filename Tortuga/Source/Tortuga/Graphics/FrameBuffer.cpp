#include "FrameBuffer.h"

namespace Tortuga
{
FrameBuffer::FrameBuffer(Device *device, Swapchain *swapchain, RenderPass *renderPass, std::vector<VkImageView> images)
{
    this->_device = device;
    auto swapchainExtent = swapchain->GetExtent2D();

    auto frameBufferInfo = VkFramebufferCreateInfo();
    {
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = renderPass->GetRenderPass();
        frameBufferInfo.attachmentCount = images.size();
        frameBufferInfo.pAttachments = images.data();
        frameBufferInfo.width = swapchainExtent.width;
        frameBufferInfo.height = swapchainExtent.height;
        frameBufferInfo.layers = 1;
    }

    if (vkCreateFramebuffer(_device->GetVirtualDevice(), &frameBufferInfo, nullptr, &_frameBuffer) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create frame buffer!");
    }
}

FrameBuffer::~FrameBuffer()
{
    vkDestroyFramebuffer(_device->GetVirtualDevice(), _frameBuffer, nullptr);
}
}; // namespace Tortuga