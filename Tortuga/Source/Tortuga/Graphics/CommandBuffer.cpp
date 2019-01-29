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

    //Setup semaphores
    _imageAvailableSemaphore.resize(MAX_FRAMES_QUEUED);
    _imageFinishedSemaphore.resize(MAX_FRAMES_QUEUED);
    _fences.resize(MAX_FRAMES_QUEUED);
    auto semaphoreInfo = VkSemaphoreCreateInfo();
    {
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    }
    auto fenceInfo = VkFenceCreateInfo();
    {
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    for (uint32_t i = 0; i < MAX_FRAMES_QUEUED; i++)
    {
        if (vkCreateSemaphore(_device->GetVirtualDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphore[i]) != VK_SUCCESS)
        {
            Console::Error("Failed to create semaphore for command buffer!");
        }
        if (vkCreateSemaphore(_device->GetVirtualDevice(), &semaphoreInfo, nullptr, &_imageFinishedSemaphore[i]) != VK_SUCCESS)
        {
            Console::Error("Failed to create semaphore for command buffer!");
        }
        if (vkCreateFence(_device->GetVirtualDevice(), &fenceInfo, nullptr, &_fences[i]) != VK_SUCCESS)
        {
            Console::Error("Failed to create fence for command buffer!");
        }
    }
    _currentFrame = 0;

    vertexBuffer = new Buffer(_device, sizeof(vertices[0]) * vertices.size(), Buffer::VERTEX_BUFFER_FLAGS);
    vertexBuffer->Fill(vertices);

    indexBuffer = new Buffer(_device, sizeof(indices[0]) * indices.size(), Buffer::INDEX_BUFFER_FLAGS);
    indexBuffer->Fill(indices);
}

CommandBuffer::~CommandBuffer()
{
    delete indexBuffer;
    delete vertexBuffer;
    //Wait for device to be free
    vkQueueWaitIdle(_device->GetGraphicsQueue());
    vkQueueWaitIdle(_device->GetPresentQueue());
    vkDeviceWaitIdle(_device->GetVirtualDevice());

    //Destroy semaphores and free command buffer
    for (uint32_t i = 0; i < MAX_FRAMES_QUEUED; i++)
    {
        vkDestroySemaphore(_device->GetVirtualDevice(), _imageAvailableSemaphore[i], nullptr);
        vkDestroySemaphore(_device->GetVirtualDevice(), _imageFinishedSemaphore[i], nullptr);
        vkDestroyFence(_device->GetVirtualDevice(), _fences[i], nullptr);
    }
    vkFreeCommandBuffers(_device->GetVirtualDevice(), _device->GetCommandPool(), _commandBuffer.size(), _commandBuffer.data());
}

void CommandBuffer::SetupDrawCall()
{
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

        std::vector<VkBuffer> buffers = {vertexBuffer->GetDeviceBuffer()};
        std::vector<VkDeviceSize> offsets = {0};

        vkCmdBindVertexBuffers(_commandBuffer[i], 0, 1, buffers.data(), offsets.data());
        vkCmdBindIndexBuffer(_commandBuffer[i], indexBuffer->GetDeviceBuffer(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(_commandBuffer[i], indices.size(), 1, 0, 0, 0);

        //Exit rendering
        vkCmdEndRenderPass(_commandBuffer[i]);
        if (vkEndCommandBuffer(_commandBuffer[i]) != VK_SUCCESS)
        {
            Console::Error("Failed to end command buffer");
        }
    }
}

void CommandBuffer::Render()
{
    vkWaitForFences(_device->GetVirtualDevice(), 1, &_fences[_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(_device->GetVirtualDevice(), 1, &_fences[_currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(_device->GetVirtualDevice(), _swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), _imageAvailableSemaphore[_currentFrame], VK_NULL_HANDLE, &imageIndex);

    std::vector<VkSemaphore> signalSemaphores = {_imageFinishedSemaphore[_currentFrame]};
    std::vector<VkSemaphore> waitSemaphores = {_imageAvailableSemaphore[_currentFrame]};
    std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    std::vector<VkSwapchainKHR> swapchains = {_swapchain->GetSwapchain()};

    auto submitInfo = VkSubmitInfo();
    {
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffer[imageIndex];
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();
    }
    if (vkQueueSubmit(_device->GetGraphicsQueue(), 1, &submitInfo, _fences[_currentFrame]) != VK_SUCCESS)
    {
        Console::Error("Failed to submit render queue");
    }

    //Submit the results back to swap chain
    auto presentInfo = VkPresentInfoKHR();
    {
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = signalSemaphores.size();
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.swapchainCount = swapchains.size();
        presentInfo.pSwapchains = swapchains.data();
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
    }
    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_QUEUED;
    vkQueuePresentKHR(_device->GetPresentQueue(), &presentInfo);
}
} // namespace Tortuga