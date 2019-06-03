#include "./Swapchain.h"

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
    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
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

VulkanSwapchain CreateSwapchain(VulkanDevice device, Window window) {
  auto data = VulkanSwapchain();
  data.VirtualDevice = device.VirtualDevice;
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
  vkCreateSwapchainKHR(device.VirtualDevice, &swapchainInfo, nullptr,
                       &data.Swapchain);

  uint32_t imageCount;
  vkGetSwapchainImagesKHR(device.VirtualDevice, data.Swapchain, &imageCount,
                          nullptr);
  data.Images.resize(imageCount);
  vkGetSwapchainImagesKHR(device.VirtualDevice, data.Swapchain, &imageCount,
                          data.Images.data());

  data.ImageViews.resize(imageCount);
  for (uint32_t i = 0; i < imageCount; i++) {
    auto imageViewInfo = VkImageViewCreateInfo();
    {
      imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewInfo.image = data.Images[i];
      imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      imageViewInfo.format = data.SurfaceFormat.format;
      imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageViewInfo.subresourceRange.baseMipLevel = 0;
      imageViewInfo.subresourceRange.levelCount = 1;
      imageViewInfo.subresourceRange.baseArrayLayer = 0;
      imageViewInfo.subresourceRange.layerCount = 1;
      if (vkCreateImageView(device.VirtualDevice, &imageViewInfo, nullptr,
                            &data.ImageViews[i]) != VK_SUCCESS) {
        Console::Fatal(
            "Failed to create Image views for swapchains on device: {0}",
            Console::Arguments() << device.Properties.deviceName);
      }
    }
  }
  return data;
}
void DestroySwapchain(VulkanSwapchain data) {
  for (uint32_t i = 0; i < data.ImageViews.size(); i++) {
    vkDestroyImageView(data.VirtualDevice, data.ImageViews[i], nullptr);
  }
  vkDestroySwapchainKHR(data.VirtualDevice, data.Swapchain, nullptr);
}
} // namespace Graphics
} // namespace Tortuga