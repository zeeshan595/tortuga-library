#ifndef _DATA_STRUCTURES
#define _DATA_STRUCTURES

#include "../../Core.h"
#include "../../Console.h"

struct VulkanData
{
  std::vector<const char *> InstanceExtensions;
  std::vector<const char *> ValidationLayers;
  VkInstance Instance;
  VkDebugUtilsMessengerEXT Debugger;
};

struct QueueFamilyIndices
{
  std::optional<uint32_t> GraphicsFamily;

  static bool IsComplete(QueueFamilyIndices indices)
  {
    return indices.GraphicsFamily.has_value();
  }
};

struct DeviceData
{
  QueueFamilyIndices QueueFamilies;
  VkPhysicalDeviceProperties Properties;
  VkPhysicalDeviceFeatures Features;
  VkPhysicalDevice PhysicalDevice;
  VkDevice Device;
  uint32_t Score;
};

#endif