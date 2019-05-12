#ifndef _VULKAN_INSTANCE
#define _VULKAN_INSTANCE

#include <vector>
#include <vulkan/vulkan.h>

#include "../Console.h"
#include "VulkanErrorHandler.h"
#include "Window.h"

namespace Tortuga {
namespace Graphics {
struct VulkanInstance {
  VkInstance Instance;
  std::vector<VkPhysicalDevice> PhysicalDevices;
};

VulkanInstance CreateVulkanInstance();
void DestroyVulkanInstance(VulkanInstance instance);

} // namespace Graphics
} // namespace Tortuga

#endif