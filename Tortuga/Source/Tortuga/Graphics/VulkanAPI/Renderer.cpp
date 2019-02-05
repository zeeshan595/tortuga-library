#include "Renderer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
Renderer::Renderer(Device *device, Swapchain *swapchain, RenderPass *renderPass, FrameBuffer *frameBuffer)
{
    _renderPass = renderPass;
    _frameBuffersSize = frameBuffer->GetFramebuffers().size();
    _device = device;
    _swapchain = swapchain;
    _frameBuffer = frameBuffer;

    _commandPool = new CommandPool(_device);
    _commandBuffers = new CommandBuffer(_device, _commandPool, _frameBuffersSize, true);

    //Setup semaphores
    _imageAvailableSemaphore.resize(MAX_FRAMES_QUEUED);
    _imageFinishedSemaphore.resize(MAX_FRAMES_QUEUED);
    _fences.resize(MAX_FRAMES_QUEUED);
    for (uint32_t i = 0; i < MAX_FRAMES_QUEUED; i++)
    {
        _imageAvailableSemaphore[i] = new Semaphore(_device);
        _imageFinishedSemaphore[i] = new Semaphore(_device);
        _fences[i] = new Fence(_device);
    }
    _currentFrame = 0;
}

Renderer::~Renderer()
{
    //Destroy semaphores and free command buffer
    for (uint32_t i = 0; i < MAX_FRAMES_QUEUED; i++)
    {
        delete _imageAvailableSemaphore[i];
        delete _imageFinishedSemaphore[i];
        delete _fences[i];
    }
    delete _commandBuffers;
    delete _commandPool;
}

void Renderer::RecordCommandBuffers(std::vector<CommandBuffer *> secondaryBuffers)
{
    //Tell GPU how to render
    for (uint32_t i = 0; i < _frameBuffersSize; i++)
    {
        _commandBuffers->BeginCommandBuffer(i);
        _commandBuffers->BeginRenderPass(i, _swapchain, _renderPass, _frameBuffer->GetFramebuffers()[i], VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        for (uint32_t j = 0; j < secondaryBuffers.size(); j++)
        {
            auto tempCommandBuffers = secondaryBuffers[j]->GetCommandBuffers();
            vkCmdExecuteCommands(_commandBuffers->GetCommandBuffers()[i], tempCommandBuffers.size(), tempCommandBuffers.data());
        }
        _commandBuffers->EndRenderPass(i);
        _commandBuffers->EndCommandBuffer(i);
    }
}

void Renderer::RenderFrame()
{
    vkWaitForFences(_device->GetVirtualDevice(), 1, &_fences[_currentFrame]->GetFence(), VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(_device->GetVirtualDevice(), 1, &_fences[_currentFrame]->GetFence());

    uint32_t imageIndex;
    vkAcquireNextImageKHR(_device->GetVirtualDevice(), _swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), _imageAvailableSemaphore[_currentFrame]->GetSemaphore(), VK_NULL_HANDLE, &imageIndex);

    std::vector<VkSemaphore> signalSemaphores = {_imageFinishedSemaphore[_currentFrame]->GetSemaphore()};
    std::vector<VkSemaphore> waitSemaphores = {_imageAvailableSemaphore[_currentFrame]->GetSemaphore()};
    std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    std::vector<VkSwapchainKHR> swapchains = {_swapchain->GetSwapchain()};

    auto submitInfo = VkSubmitInfo();
    {
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = waitSemaphores.size();
        submitInfo.pWaitSemaphores = waitSemaphores.data();
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffers->GetCommandBuffers()[imageIndex];
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();
    }
    if (vkQueueSubmit(_device->GetGraphicsQueue(), 1, &submitInfo, _fences[_currentFrame]->GetFence()) != VK_SUCCESS)
    {
        Console::Fatal("Failed to submit render queue");
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
    if (vkQueuePresentKHR(_device->GetPresentQueue(), &presentInfo) != VK_SUCCESS)
    {
        Console::Fatal("Failed to display image!");
    }
    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_QUEUED;
}

void Renderer::WaitForGPUIdle()
{
    //Wait for device to be free
    vkQueueWaitIdle(_device->GetGraphicsQueue());
    vkQueueWaitIdle(_device->GetPresentQueue());
    vkDeviceWaitIdle(_device->GetVirtualDevice());
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga