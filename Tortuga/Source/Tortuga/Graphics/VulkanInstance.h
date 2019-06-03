#ifndef _VULKAN_INSTANCE
#define _VULKAN_INSTANCE

#include <vector>
#include <vulkan/vulkan.h>

#include "../Console.h"
#include "VulkanErrorHandler.h"
#include "Window.h"
#include "VulkanDevice.h"

namespace Tortuga {
namespace Graphics {
struct VulkanInstance {
  VkInstance Instance = VK_NULL_HANDLE;
  std::vector<VulkanDevice> Devices;
  VkDebugUtilsMessengerEXT Debugger = VK_NULL_HANDLE;
};

VulkanInstance CreateVulkanInstance();
void DestroyVulkanInstance(VulkanInstance instance);

} // namespace Graphics
} // namespace Tortuga

#endif