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
  uint32_t Index = -1;
  uint32_t Count = 0;
  bool CanPresent = false;
};
struct DeviceQueueFamilies
{
  DeviceQueueFamilyIndex Compute;
  DeviceQueueFamilyIndex Graphics;

  static const uint8_t SIZE = 2;
  static std::vector<DeviceQueueFamilyIndex> GetIndices(DeviceQueueFamilies queueFamilies)
  {
    return {queueFamilies.Compute, queueFamilies.Graphics};
  }
};
struct DeviceQueues
{
  std::vector<VkQueue> Compute;
  std::vector<VkQueue> Graphics;
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

float GetDeviceScore(VkPhysicalDeviceProperties properties, VkPhysicalDeviceFeatures features)
{
  uint32_t score = 0;
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += properties.limits.maxImageDimension2D;
  return score;
}

DeviceQueueFamilies FindDeviceQueueIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR &surface)
{
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

  DeviceQueueFamilies data = {};

  for (uint32_t i = 0; i < queueFamilies.size(); i++)
  {
    auto queueFamily = queueFamilies[i];
    if (queueFamily.queueCount <= 0)
      continue;

    VkBool32 isSupported = false;
    ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &isSupported));

    if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
      data.Compute.Index = i;
      data.Compute.CanPresent = isSupported;
      data.Compute.Count = queueFamilies[i].queueCount;
    }
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      data.Graphics.Index = i;
      data.Graphics.CanPresent = isSupported;
      data.Graphics.Count = queueFamilies[i].queueCount;
    }

    if (data.Compute.Index != -1 && data.Graphics.Index != -1)
      break;
  }

  return data;
}

bool IsExtensionsSupported(VkPhysicalDevice physicalDevice, std::vector<const char *> extensions)
{
  uint32_t extensionCount;
  ErrorCheck::Callback(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  ErrorCheck::Callback(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data()));

  std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
  for (const auto &extension : availableExtensions)
    requiredExtensions.erase(extension.extensionName);

  return requiredExtensions.empty();
}

Device Create(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
  Device data = {};
  data.PhysicalDevice = physicalDevice;

  vkGetPhysicalDeviceProperties(physicalDevice, &data.Properties);
  vkGetPhysicalDeviceFeatures(physicalDevice, &data.Features);
  data.Score = GetDeviceScore(data.Properties, data.Features);
  if (data.Score <= 0)
  {
    Console::Warning("Incompatible device: {0}", data.Properties.deviceName);
    return data;
  }

  //Device extensions required
  const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  if (!IsExtensionsSupported(physicalDevice, deviceExtensions))
  {
    Console::Warning("Device does not support required extensions: {0}", data.Properties.deviceName);
    return data;
  }

  data.QueueFamilies = FindDeviceQueueIndices(physicalDevice, surface);

  float queuePriority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  auto familyIndices = DeviceQueueFamilies::GetIndices(data.QueueFamilies);
  for (uint32_t i = 0; i < familyIndices.size(); i++)
  {
    VkDeviceQueueCreateInfo queueInfo = {};
    {
      queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueInfo.queueFamilyIndex = familyIndices[i].Index;
      queueInfo.queueCount = familyIndices[i].Count;
      queueInfo.pQueuePriorities = &queuePriority;
    }
    queueCreateInfos.push_back(queueInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo deviceInfo = {};
  {
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = nullptr;
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;
    deviceInfo.enabledExtensionCount = deviceExtensions.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  }

  ErrorCheck::Callback(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &data.Device));

  for (uint32_t i = 0; i < data.QueueFamilies.Compute.Count; i++)
  {
    VkQueue queue;
    vkGetDeviceQueue(data.Device, data.QueueFamilies.Compute.Index, i, &queue);
    data.Queues.Compute.push_back(queue);
  }

  for (uint32_t i = 0; i < data.QueueFamilies.Graphics.Count; i++)
  {
    VkQueue queue;
    vkGetDeviceQueue(data.Device, data.QueueFamilies.Graphics.Index, i, &queue);
    data.Queues.Graphics.push_back(queue);
  }

  data.CanPresent = data.QueueFamilies.Compute.CanPresent || data.QueueFamilies.Graphics.CanPresent;
  data.IsDeviceCompatible = true;
  return data;
}
void Destroy(Device data)
{
  vkDestroyDevice(data.Device, nullptr);
}

uint32_t FindMemoryType(Device device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(device.PhysicalDevice, &memoryProperties);

  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
  {
    if ((typeFilter & (1 << i)))
      if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
  }

  Console::Fatal("Failed to find suitable memory type on device: {0}", device.Properties.deviceName);
  return 0;
}
} // namespace Device
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif