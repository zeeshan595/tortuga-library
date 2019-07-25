#ifndef _VULKAN_DEVICE
#define _VULKAN_DEVICE

#include <vulkan/vulkan.h>
#include <set>

#include "./ErrorCheck.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Device
{
struct DeviceQueueFamilyIndex
{
  uint32_t Index = 0;
  uint32_t Count = 0;
  bool CanPresent = false;
};
struct DeviceQueueFamilies
{
  DeviceQueueFamilyIndex Compute;
  DeviceQueueFamilyIndex Graphics;
  DeviceQueueFamilyIndex Transfer;

  static std::vector<DeviceQueueFamilyIndex> GetIndices(DeviceQueueFamilies queueFamilies);
};
struct DeviceQueues
{
  std::vector<VkQueue> Compute;
  std::vector<VkQueue> Graphics;
  std::vector<VkQueue> Transfer;
  VkQueue Present;
  uint32_t PresentIndex;
};
struct Device
{
  bool IsDeviceCompatible = false;
  bool CanPresent = false;
  VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
  VkDevice Device = VK_NULL_HANDLE;
  VkPhysicalDeviceProperties Properties;
  VkPhysicalDeviceFeatures Features;
  DeviceQueueFamilies QueueFamilies;
  DeviceQueues Queues;
  uint32_t Score = 1;
};

float GetDeviceScore(VkPhysicalDeviceProperties properties, VkPhysicalDeviceFeatures features);
DeviceQueueFamilies FindDeviceQueueIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR &surface);
bool IsExtensionsSupported(VkPhysicalDevice physicalDevice, std::vector<const char *> extensions);
Device Create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
void Destroy(Device data);
uint32_t FindMemoryType(Device device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void WaitForQueue(VkQueue queue);
void WaitForDevice(Device data);
} // namespace Device
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif