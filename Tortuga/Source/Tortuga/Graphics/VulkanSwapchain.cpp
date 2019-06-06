#include "./VulkanSwapchain.h"

namespace Tortuga {
namespace Graphics {
SwapChainSupportDetails QuerySwapChainSupport(VulkanDevice device,
                                              VkSurfaceKHR surface) {
  SwapChainSupportDetails details;
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.PhysicalDevice, surface,
                                                &details.Capabilities) !=
      VK_SUCCESS) {
    Console::Fatal("Failed to get swapchain capabilities on device: {0}",
                   Console::Arguments() << device.Properties.deviceName);
  }

  uint32_t formatCount;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(device.PhysicalDevice, surface,
                                           &formatCount,
                                           nullptr) != VK_SUCCESS) {
    Console::Fatal("Failed to get surface formats support on device: {0}",
                   Console::Arguments() << device.Properties.deviceName);
  }
  if (formatCount != 0) {
    details.Formats.resize(formatCount);
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(
            device.PhysicalDevice, surface, &formatCount,
            details.Formats.data()) != VK_SUCCESS) {
      Console::Fatal("Failed to get surface formats support on device: {0}",
                     Console::Arguments() << device.Properties.deviceName);
    }
  }

  uint32_t presentModeCount;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(device.PhysicalDevice, surface,
                                                &presentModeCount,
                                                nullptr) != VK_SUCCESS) {
    Console::Fatal("Failed to get surface formats support on device: {0}",
                   Console::Arguments() << device.Properties.deviceName);
  }
  if (presentModeCount != 0) {
    details.PresentModes.resize(presentModeCount);
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(
            device.PhysicalDevice, surface, &presentModeCount,
            details.PresentModes.data()) != VK_SUCCESS) {
      Console::Fatal("Failed to get surface formats support on device: {0}",
                     Console::Arguments() << device.Properties.deviceName);
    }
  }

  if (details.Formats.empty() || details.PresentModes.empty()) {
    Console::Fatal("Failed to get swap chain details on device: {0}",
                   Console::Arguments() << device.Properties.deviceName);
  }

  return details;
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  if (availableFormats.size() == 1 &&
      availableFormats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_R32G32B32A32_SFLOAT, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_R32G32B32A32_SFLOAT &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> availablePresentModes) {
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      bestMode = availablePresentMode;
    }
  }
  return bestMode;
}

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            uint32_t width, uint32_t height) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    VkExtent2D actualExtent = {width, height};

    actualExtent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

VulkanSwapchain CreateVulkanSwapchain(VulkanDevice device, Window window) {
  auto data = VulkanSwapchain();
  data.VirtualDevice = device.VirtualDevice;
  data.DevicePresentQueue = device.PresentQueue;
  data.SupportDetails = QuerySwapChainSupport(device, window.WindowSurface);
  data.SurfaceFormat = ChooseSwapSurfaceFormat(data.SupportDetails.Formats);
  data.PresentMode = ChooseSwapPresentMode(data.SupportDetails.PresentModes);
  data.Extent = ChooseSwapExtent(data.SupportDetails.Capabilities, window.Width,
                                 window.Height);

  VkBool32 isSupported = false;
  ErrorCheck(vkGetPhysicalDeviceSurfaceSupportKHR(
      device.PhysicalDevice, device.QueueFamilies.ComputeFamily.value(),
      window.WindowSurface, &isSupported));
  if (!isSupported) {
    Console::Fatal("Graphics queue not found when creating swapchain?");
  }
  ErrorCheck(vkGetPhysicalDeviceSurfaceSupportKHR(
      device.PhysicalDevice, device.QueueFamilies.PresentFamily.value(),
      window.WindowSurface, &isSupported));
  if (!isSupported) {
    Console::Fatal("Present queue not found when creating swapchain?");
  }

  data.ImageCount = data.SupportDetails.Capabilities.minImageCount + 1;
  if (data.SupportDetails.Capabilities.maxImageCount > 0 &&
      data.ImageCount > data.SupportDetails.Capabilities.maxImageCount) {
    data.ImageCount = data.SupportDetails.Capabilities.maxImageCount;
  }

  auto swapchainInfo = VkSwapchainCreateInfoKHR();
  {
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = window.WindowSurface;
    swapchainInfo.minImageCount = data.ImageCount;
    swapchainInfo.imageFormat = data.SurfaceFormat.format;
    swapchainInfo.imageColorSpace = data.SurfaceFormat.colorSpace;
    swapchainInfo.imageExtent = data.Extent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    uint32_t queueFamilyIndices[] = {
        device.QueueFamilies.ComputeFamily.value(),
        device.QueueFamilies.PresentFamily.value()};

    if (device.QueueFamilies.ComputeFamily !=
        device.QueueFamilies.PresentFamily) {
      swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapchainInfo.queueFamilyIndexCount = 2;
      swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchainInfo.queueFamilyIndexCount = 0;     // Optional
      swapchainInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    swapchainInfo.preTransform =
        data.SupportDetails.Capabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = data.PresentMode;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
  }
  ErrorCheck(vkCreateSwapchainKHR(device.VirtualDevice, &swapchainInfo, nullptr,
                                  &data.Swapchain));

  uint32_t imageCount;
  ErrorCheck(vkGetSwapchainImagesKHR(device.VirtualDevice, data.Swapchain,
                                     &imageCount, nullptr));
  data.Images.resize(imageCount);
  ErrorCheck(vkGetSwapchainImagesKHR(device.VirtualDevice, data.Swapchain,
                                     &imageCount, data.Images.data()));
  return data;
}
void DestroyVulkanSwapchain(VulkanSwapchain data) {
  vkDestroySwapchainKHR(data.VirtualDevice, data.Swapchain, nullptr);
}
void SwapchainPresentImage(VulkanSwapchain swapchain, uint32_t imageIndex,
                           std::vector<VkSemaphore> semaphores) {
  std::vector<VkSwapchainKHR> swapChains = {swapchain.Swapchain};
  auto presentInfo = VkPresentInfoKHR();
  {
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = semaphores.size();
    presentInfo.pWaitSemaphores = semaphores.data();
    presentInfo.swapchainCount = swapChains.size();
    presentInfo.pSwapchains = swapChains.data();
    presentInfo.pImageIndices = &imageIndex;
  }
  Graphics::ErrorCheck(
      vkQueuePresentKHR(swapchain.DevicePresentQueue, &presentInfo));
}
void SwapchainAquireImage(VulkanSwapchain swapchain, uint32_t *imageIndex,
                          VkSemaphore semaphore,
                          VulkanFence fence) {
  ErrorCheck(vkAcquireNextImageKHR(swapchain.VirtualDevice, swapchain.Swapchain,
                                   std::numeric_limits<uint64_t>::max(),
                                   VK_NULL_HANDLE, fence.Fence, imageIndex));
}
} // namespace Graphics
} // namespace Tortuga