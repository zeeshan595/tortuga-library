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
  std::vector<VkImage> _swapchainImages;
  std::vector<VkImageView> _swapchainImageViews;
  uint32_t _width;
  uint32_t _height;

public:
  Swapchain(Device *device, uint32_t width, uint32_t height, VkSwapchainKHR previousSwapchain = VK_NULL_HANDLE);
  ~Swapchain();

  static const VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  static const VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
  static const VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height);

  Device *GetDevice() { return _device; }
  VkSwapchainKHR GetSwapchain() { return _swapchain; }
  VkExtent2D GetExtent2D() { return _extent; }
  VkSurfaceFormatKHR GetSurfaceFormat() { return _surfaceFormat; }
  VkPresentModeKHR GetPresentMode() { return _presentMode; }
  std::vector<VkImage> GetSwapchainRawImages() { return _swapchainImages; }
  uint32_t GetWidth() { return _width; }
  uint32_t GetHeight() { return _height; }
};
} // namespace Tortuga

#endif