#include "CommandBuffer.h"

namespace Tortuga
{
CommandBuffer::CommandBuffer(Pipeline *pipeline, std::vector<Framebuffer *> frameBuffers)
{
    _pipeline = pipeline;
    _device = pipeline->GetDevice();
    _swapchain = pipeline->GetSwapchain();
    _frameBuffers = frameBuffers;
    _commandBuffer.resize(frameBuffers.size());

    auto allocInfo = VkCommandBufferAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _device->GetCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = _commandBuffer.size();
    }
    if (vkAllocateCommandBuffers(_device->GetVirtualDevice(), &allocInfo, _commandBuffer.data()) != VK_SUCCESS)
    {
        Console::Error("Failed to allocate command buffer on device!");
    }

    //Tell GPU how to render
    for (uint32_t i = 0; i < _commandBuffer.size(); i++)
    {
        //Begin Rendering
        auto beginInfo = VkCommandBufferBeginInfo();
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(_commandBuffer[i], &beginInfo) != VK_SUCCESS)
        {
            Console::Error("Failed to begin command buffer!");
            return;
        }

        auto renderPassInfo = VkRenderPassBeginInfo();
        {
            VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = _pipeline->GetRenderPass();
            renderPassInfo.framebuffer = _frameBuffers[i]->GetFramebuffer();
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = _swapchain->GetExtent2D();
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;
        }
        vkCmdBeginRenderPass(_commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        //Actuall rendering
        vkCmdBindPipeline(_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetVulkanPipeline());
        vkCmdDraw(_commandBuffer[i], 3, 1, 0, 0);

        //Exit rendering
        vkCmdEndRenderPass(_commandBuffer[i]);
        if (vkEndCommandBuffer(_commandBuffer[i]) != VK_SUCCESS)
        {
            Console::Error("Failed to end command buffer");
        }
    }
}

CommandBuffer::~CommandBuffer()
{
    vkFreeCommandBuffers(_device->GetVirtualDevice(), _device->GetCommandPool(), _commandBuffer.size(), _commandBuffer.data());
}
} // namespace Tortuga