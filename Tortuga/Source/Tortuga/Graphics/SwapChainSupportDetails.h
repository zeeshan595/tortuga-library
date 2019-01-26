#ifndef _SWAPCHAIN_SUPPORT_DETAILS
#define _SWAPCHAIN_SUPPORT_DETAILS

#include "../Core.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif