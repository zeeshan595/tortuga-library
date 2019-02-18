#ifndef _VULKAN_API_SWAPCHAIN
#define _VULKAN_API_SWAPCHAIN

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
SwapchainData CreateSwapchain(DeviceData device, VkSurfaceKHR surface, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height);
void DestroySwapchain(SwapchainData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif