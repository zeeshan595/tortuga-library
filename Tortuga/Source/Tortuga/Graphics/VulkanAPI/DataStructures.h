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
  std::optional<uint32_t> PresentFamily;

  static bool IsComplete(QueueFamilyIndices indices)
  {
    return indices.GraphicsFamily.has_value() &&
           indices.PresentFamily.has_value();
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
  VkQueue PresentQueue;
};

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR Capabilities;
  std::vector<VkSurfaceFormatKHR> Formats;
  std::vector<VkPresentModeKHR> PresentModes;
};

struct SwapchainData
{
  VkDevice Device;
  SwapChainSupportDetails SupportDetails;
  VkSurfaceFormatKHR SurfaceFormat;
  VkPresentModeKHR PresentMode;
  VkExtent2D Extent;
  uint32_t ImageCount;
  VkSwapchainKHR Swapchain;
};

#endif