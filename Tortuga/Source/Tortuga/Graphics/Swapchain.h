#ifndef _SWAPCHAIN
#define _SWAPCHAIN

#include "./VulkanDevice.h"
#include "./Window.h"

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
  std::vector<VkImageView> ImageViews;
  uint32_t ImageCount;
  VkSurfaceFormatKHR SurfaceFormat;
  VkExtent2D Extent;
  VkPresentModeKHR PresentMode;
  SwapChainSupportDetails SupportDetails;
  VkInstance VulkanInstance;
  VkDevice VirtualDevice;
};

VulkanSwapchain CreateSwapchain(VulkanDevice device, Window window);
void DestroySwapchain(VulkanSwapchain swapchain);
} // namespace Graphics
} // namespace Tortuga

#endif