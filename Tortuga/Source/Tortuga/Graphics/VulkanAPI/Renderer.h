#ifndef _VULKAN_API_RENDERER
#define _VULKAN_API_RENDERER

#include "DataStructures.h"
#include "CommandPool.h"
#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void SubmitCommands(RendererData data, std::vector<CommandBufferData> commandBuffers);
void DrawFrame(RendererData data);
RendererData CreateRenderer(DeviceData device, SwapchainData swapchain, FrameBufferData frameBuffers, RenderPassData renderPass);
void DestroyRenderer(RendererData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif