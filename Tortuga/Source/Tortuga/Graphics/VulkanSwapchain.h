#ifndef _VULKAN_SWAPCHAIN
#define _VULKAN_SWAPCHAIN

#include "./VulkanDevice.h"
#include "./Window.h"
#include "./VulkanFence.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR Capabilities;
  std::vector<VkSurfaceFormatKHR> Formats;
  std::vector<VkPresentModeKHR> PresentModes;
};
struct VulkanSwapchain {
  VkSwapchainKHR Swapchain;
  std::vector<VkImage> Images;
  uint32_t ImageCount;
  VkSurfaceFormatKHR SurfaceFormat;
  VkExtent2D Extent;
  VkPresentModeKHR PresentMode;
  SwapChainSupportDetails SupportDetails;
  VkInstance VulkanInstance;
  VkDevice VirtualDevice;
  VkQueue DevicePresentQueue;
};

VulkanSwapchain CreateVulkanSwapchain(VulkanDevice device, Window window);
void DestroyVulkanSwapchain(VulkanSwapchain swapchain);
void SwapchainAquireImage(VulkanSwapchain swapchain, uint32_t *imageIndex,
                          VkSemaphore semaphore,
                          VulkanFence fence);
void SwapchainPresentImage(VulkanSwapchain swapchain, uint32_t imageIndex,
                           std::vector<VkSemaphore> semaphores = {});
} // namespace Graphics
} // namespace Tortuga

#endif