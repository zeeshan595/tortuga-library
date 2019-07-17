#ifndef _VULKAN_SWAPCHAIN
#define _VULKAN_SWAPCHAIN

#include <vulkan/vulkan.h>

#include "./Device.hpp"
#include "./Window.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Swapchain
{
struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR Capabilities;
  std::vector<VkSurfaceFormatKHR> Formats;
  std::vector<VkPresentModeKHR> PresentModes;
};
struct Swapchain
{
  VkDevice Device = VK_NULL_HANDLE;
  VkSwapchainKHR Swapchain = VK_NULL_HANDLE;
  SwapChainSupportDetails SupportDetails;
  VkSurfaceFormatKHR SurfaceFormat;
  VkPresentModeKHR PresentMode;
  VkExtent2D Extent;
  uint32_t ImageCount;
  std::vector<VkImage> Images;
};

Swapchain Create(Device::Device device, Window::Window window, VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE);
void Destroy(Swapchain data);

SwapChainSupportDetails GetSupportDetails(Device::Device device, Window::Window window);
VkSurfaceFormatKHR ChooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);
VkPresentModeKHR ChoosePresentMode(std::vector<VkPresentModeKHR> presentModes);
VkExtent2D ChooseExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height);
} // namespace Swapchain
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif