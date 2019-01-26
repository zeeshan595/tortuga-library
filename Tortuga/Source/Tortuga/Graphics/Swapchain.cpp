#include "Swapchain.h"

namespace Tortuga
{

Swapchain::Swapchain(Device *device, uint32_t width, uint32_t height, VkSwapchainKHR previousSwapchain)
{
    this->_device = device;

    auto details = device->GetSwapchainSupportDetails();
    _surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
    _presentMode = ChooseSwapPresentMode(details.presentModes);
    _extent = ChooseSwapExtent(details.capabilities, width, height);

    uint32_t imageCount = details.capabilities.minImageCount + 1;
    {
        if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
            imageCount = details.capabilities.maxImageCount;
    }

    auto swapChainCreateInfo = VkSwapchainCreateInfoKHR();
    {
        swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainCreateInfo.surface = device->GetSurface();
        swapChainCreateInfo.minImageCount = imageCount;
        swapChainCreateInfo.imageFormat = _surfaceFormat.format;
        swapChainCreateInfo.imageColorSpace = _surfaceFormat.colorSpace;
        swapChainCreateInfo.imageExtent = _extent;
        swapChainCreateInfo.imageArrayLayers = 1;
        swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        //Queue family indices
        auto indices = device->GetQueueFamilyIndices();
        std::vector<uint32_t> queueFamilyIndices = indices.GetFamiliesArray();
        if (indices.graphicsFamily != indices.presentFamily)
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapChainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
            swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapChainCreateInfo.queueFamilyIndexCount = 0;
            swapChainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapChainCreateInfo.preTransform = details.capabilities.currentTransform;
        swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainCreateInfo.presentMode = _presentMode;
        swapChainCreateInfo.clipped = VK_TRUE;
        swapChainCreateInfo.oldSwapchain = previousSwapchain;
    }

    if (vkCreateSwapchainKHR(device->GetVirtualDevice(), &swapChainCreateInfo, nullptr, &_swapchain) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create swapchain!");
    }
}

Swapchain::~Swapchain()
{
    vkDestroySwapchainKHR(_device->GetVirtualDevice(), _swapchain, nullptr);
}

const VkSurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto &format : availableFormats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }
    return availableFormats[0];
}

const VkPresentModeKHR Swapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
        else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

const VkExtent2D Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {width, height};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}
} // namespace Tortuga