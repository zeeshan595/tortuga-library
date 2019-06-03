#ifndef _VULKAN_DEVICES
#define _VULKAN_DEVICES

#include "../Console.h"
#include "./VulkanErrorHandler.h"

#include <optional>
#include <set>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct QueueFamilyIndices {
  std::optional<uint32_t> GraphicsFamily;
  std::optional<uint32_t> PresentFamily;

  static bool IsComplete(QueueFamilyIndices indices) {
    return indices.GraphicsFamily.has_value() &&
           indices.PresentFamily.has_value();
  }
};
struct VulkanDevice {
  QueueFamilyIndices QueueFamilies;
  VkPhysicalDeviceProperties Properties;
  VkPhysicalDeviceFeatures Features;
  VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
  VkDevice VirtualDevice = VK_NULL_HANDLE;
  VkQueue PresentQueue = VK_NULL_HANDLE;
  VkQueue GraphicQueue = VK_NULL_HANDLE;
  uint32_t Score;
  bool IsReady = false;
};

VulkanDevice CreateDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                    VkInstance vulkanInstance, std::vector<const char*> validationLayers);
void DestroyDevice(VulkanDevice device);

} // namespace Graphics
} // namespace Tortuga

#endif