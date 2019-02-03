#ifndef _SWAPCHAIN_SUPPORT_DETAILS
#define _SWAPCHAIN_SUPPORT_DETAILS

#include "../../Core.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif