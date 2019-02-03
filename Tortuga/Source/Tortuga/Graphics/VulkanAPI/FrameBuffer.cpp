#include "FrameBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
FrameBuffer::FrameBuffer(Device *device, Swapchain *swapchain, RenderPass *renderPass)
{
    this->_device = device;

    auto swpachainImages = swapchain->GetSwapchainImageViews();
    auto swapchainExtent = swapchain->GetExtent2D();

    _frameBuffer.resize(swpachainImages.size());
    for (uint32_t i = 0; i < _frameBuffer.size(); i++)
    {
        std::vector<VkImageView> attachments = {swpachainImages[i]};

        auto frameBufferInfo = VkFramebufferCreateInfo();
        {
            frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frameBufferInfo.renderPass = renderPass->GetRenderPass();
            frameBufferInfo.attachmentCount = attachments.size();
            frameBufferInfo.pAttachments = attachments.data();
            frameBufferInfo.width = swapchainExtent.width;
            frameBufferInfo.height = swapchainExtent.height;
            frameBufferInfo.layers = 1;
        }

        if (vkCreateFramebuffer(_device->GetVirtualDevice(), &frameBufferInfo, nullptr, &_frameBuffer[i]) != VK_SUCCESS)
        {
            Console::Fatal("Failed to create frame buffer!");
        }
    }
}

FrameBuffer::~FrameBuffer()
{
    for (uint32_t i = 0; i < _frameBuffer.size(); i++)
        vkDestroyFramebuffer(_device->GetVirtualDevice(), _frameBuffer[i], nullptr);
}

void FrameBuffer::CreateDepthImageView()
{
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga