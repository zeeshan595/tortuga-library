#ifndef _VULKAN_API_FRAME_BUFFERS
#define _VULKAN_API_FRAME_BUFFERS

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
FrameBufferData CreateFrameBuffers(DeviceData device, SwapchainData swapchain, RenderPassData renderPass);
void DestroyFrameBuffers(FrameBufferData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif