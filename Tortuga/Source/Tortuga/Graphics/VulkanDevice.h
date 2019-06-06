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
  std::optional<uint32_t> ComputeFamily;
  std::optional<uint32_t> PresentFamily;

  std::vector<uint32_t> Values() {
    return {ComputeFamily.value(), PresentFamily.value()};
  }
  static bool IsComplete(QueueFamilyIndices indices) {
    return indices.ComputeFamily.has_value() &&
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
  VkQueue ComputeQueue = VK_NULL_HANDLE;
  uint32_t Score;
  bool IsReady = false;
};
enum VulkanQueueType { VULKAN_QUEUE_TYPE_COMPUTE, VULKAN_QUEUE_TYPE_PRESENT };

VulkanDevice CreateVulkanDevice(VkPhysicalDevice physicalDevice,
                                VkSurfaceKHR surface, VkInstance vulkanInstance,
                                std::vector<const char *> validationLayers);
void DestroyVulkanDevice(VulkanDevice device);
void DeviceQueueWaitForIdle(VulkanDevice data, VulkanQueueType queueType);
} // namespace Graphics
} // namespace Tortuga

#endif