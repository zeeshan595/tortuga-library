#include "./Swapchain.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Swapchain
{
SwapChainSupportDetails GetSupportDetails(Device::Device device, Window::Window window)
{
  SwapChainSupportDetails data = {};

  ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.PhysicalDevice, window.Surface, &data.Capabilities));

  uint32_t formatCount;
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceFormatsKHR(device.PhysicalDevice, window.Surface, &formatCount, nullptr));
  data.Formats.resize(formatCount);
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceFormatsKHR(device.PhysicalDevice, window.Surface, &formatCount, data.Formats.data()));

  uint32_t presentModeCount;
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfacePresentModesKHR(device.PhysicalDevice, window.Surface, &presentModeCount, nullptr));
  data.PresentModes.resize(presentModeCount);
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfacePresentModesKHR(device.PhysicalDevice, window.Surface, &presentModeCount, data.PresentModes.data()));

  return data;
}

VkSurfaceFormatKHR ChooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats)
{
  if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
    return {VK_FORMAT_R8G8B8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

  for (uint32_t i = 0; i < formats.size(); i++)
  {
    if (formats[i].format == VK_FORMAT_R8G8B8_UNORM)
      if (formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        return formats[i];
  }

  return formats[0];
}

VkPresentModeKHR ChoosePresentMode(std::vector<VkPresentModeKHR> presentModes)
{
  VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
  for (uint32_t i = 0; i < presentModes.size(); i++)
  {
    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
      return presentModes[i];

    if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
      bestMode = presentModes[i];
  }
  return bestMode;
}

VkExtent2D ChooseExtent(VkSurfaceCapabilitiesKHR capabilities, uint32_t width, uint32_t height)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return capabilities.currentExtent;

  VkExtent2D actualExtent = {width, height};

  actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
  actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

  return actualExtent;
}

Swapchain Create(Device::Device device, Window::Window window, VkSwapchainKHR oldSwapchain)
{
  Swapchain data = {};
  data.Device = device.Device;
  data.SupportDetails = GetSupportDetails(device, window);
  data.SurfaceFormat = ChooseSurfaceFormat(data.SupportDetails.Formats);
  data.PresentMode = ChoosePresentMode(data.SupportDetails.PresentModes);
  data.Extent = ChooseExtent(data.SupportDetails.Capabilities, window.Width, window.Height);

  VkBool32 isComputeSupported = false;
  VkBool32 isGraphicsSupported = false;
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceSupportKHR(device.PhysicalDevice, device.QueueFamilies.Compute.Index, window.Surface, &isComputeSupported));
  ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceSupportKHR(device.PhysicalDevice, device.QueueFamilies.Compute.Index, window.Surface, &isGraphicsSupported));
  if (!isComputeSupported && !isGraphicsSupported)
    Console::Fatal("This device does not have any present queues");

  data.ImageCount = data.SupportDetails.Capabilities.minImageCount + 1;
  if (data.SupportDetails.Capabilities.maxImageCount > 0)
    if (data.ImageCount > data.SupportDetails.Capabilities.maxImageCount)
      data.ImageCount = data.SupportDetails.Capabilities.maxImageCount;

  VkSwapchainCreateInfoKHR swapchainInfo = {};
  {
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.surface = window.Surface;
    swapchainInfo.minImageCount = data.ImageCount;
    swapchainInfo.imageFormat = data.SurfaceFormat.format;
    swapchainInfo.imageColorSpace = data.SurfaceFormat.colorSpace;
    swapchainInfo.imageExtent = data.Extent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.preTransform = data.SupportDetails.Capabilities.currentTransform;
    swapchainInfo.presentMode = data.PresentMode;
    swapchainInfo.clipped = true;
    swapchainInfo.oldSwapchain = oldSwapchain;
  }

  ErrorCheck::Callback(vkCreateSwapchainKHR(device.Device, &swapchainInfo, nullptr, &data.Swapchain));

  data.Images.resize(data.ImageCount);
  ErrorCheck::Callback(vkGetSwapchainImagesKHR(device.Device, data.Swapchain, &data.ImageCount, data.Images.data()));

  VkFenceCreateInfo fenceInfo = {};
  {
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  }
  ErrorCheck::Callback(vkCreateFence(data.Device, &fenceInfo, nullptr, &data.Fence));

  return data;
}
void Destroy(Swapchain data)
{
  vkDestroyFence(data.Device, data.Fence, nullptr);
  vkDestroySwapchainKHR(data.Device, data.Swapchain, nullptr);
}
uint32_t AquireNextImage(Swapchain data)
{
  vkResetFences(data.Device, 1, &data.Fence);
  uint32_t imageIndex;
  ErrorCheck::Callback(vkAcquireNextImageKHR(data.Device, data.Swapchain, std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, data.Fence, &imageIndex));
  ErrorCheck::Callback(vkWaitForFences(data.Device, 1, &data.Fence, VK_TRUE, std::numeric_limits<uint64_t>::max()));
  return imageIndex;
}
Image::Image GetImage(Swapchain data, uint32_t index)
{
  Image::Image image = {};
  image.Device = data.Device;
  image.Width = data.Extent.width;
  image.Height = data.Extent.height;
  image.Image = data.Images[index];
  image.Memory = VK_NULL_HANDLE;
  return image;
}
void PresentImage(Swapchain data, uint32_t imageIndex, VkQueue Queue)
{
  std::vector<VkSwapchainKHR> swapChains = {data.Swapchain};
  auto presentInfo = VkPresentInfoKHR();
  {
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pWaitSemaphores = VK_NULL_HANDLE;
    presentInfo.swapchainCount = swapChains.size();
    presentInfo.pSwapchains = swapChains.data();
    presentInfo.pImageIndices = &imageIndex;
  }
  ErrorCheck::Callback(vkQueuePresentKHR(Queue, &presentInfo));
}
} // namespace Swapchain
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga