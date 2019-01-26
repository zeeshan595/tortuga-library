#ifndef _SWAPCHAIN
#define _SWAPCHAIN

#include "../Core.h"
#include "SwapChainSupportDetails.h"
#include "Device.h"

namespace Tortuga
{
class Swapchain
{
private:
  VkSurfaceFormatKHR _surfaceFormat;
  VkPresentModeKHR _presentMode;
  VkExtent2D _extent;
  VkSwapchainKHR _swapchain;
  Device *_device;

public:
  Swapchain(Device *device, uint32_t width, uint32_t height, VkSwapchainKHR previousSwapchain = VK_NULL_HANDLE);
  ~Swapchain();

  static const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  static const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
  static const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height);
};
} // namespace Tortuga

#endif