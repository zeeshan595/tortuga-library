#include "VulkanDevice.h"

namespace Tortuga {
namespace Graphics {
uint32_t FindMemoryType(VulkanDevice device, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(device.PhysicalDevice, &memoryProperties);

  for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) &&
        (memoryProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      return i;
    }
  }

  Console::Fatal("Failed to find suitable memory type on device: {0}",
                 device.Properties.deviceName);
  return 0;
}
bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice,
                                 std::vector<const char *> extensions) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(extensions.begin(),
                                           extensions.end());
  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}
QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice,
                                     VkSurfaceKHR &surface) {
  auto indices = QueueFamilyIndices();

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                           nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueCount <= 0)
      continue;

    if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
      indices.ComputeFamily = i;
    }

    VkBool32 presentSupport = false;
    if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface,
                                             &presentSupport) != VK_SUCCESS) {
      Console::Fatal("Failed to check device surface support");
    }
    if (presentSupport) {
      indices.PresentFamily = i;
    }

    if (QueueFamilyIndices::IsComplete(indices)) {
      break;
    }

    i++;
  }

  return indices;
}
uint32_t GetDeviceScore(VkPhysicalDeviceProperties properties,
                        VkPhysicalDeviceFeatures features) {
  uint32_t score = 0;

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += properties.limits.maxImageDimension2D;

  if (features.geometryShader == false)
    score = 0;

  return score;
}

VulkanDevice CreateDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                          VkInstance vulkanInstance,
                          std::vector<const char *> validationLayers) {
  auto data = VulkanDevice();
  data.PhysicalDevice = physicalDevice;
  if (physicalDevice == VK_NULL_HANDLE || physicalDevice == nullptr ||
      physicalDevice == NULL) {
    Console::Error("Please provide the physical device");
    return data;
  }

  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  vkGetPhysicalDeviceProperties(physicalDevice, &data.Properties);
  vkGetPhysicalDeviceFeatures(physicalDevice, &data.Features);

  data.QueueFamilies = FindQueueFamilies(physicalDevice, surface);
  if (QueueFamilyIndices::IsComplete(data.QueueFamilies) == false)
    return data;

  if (CheckDeviceExtensionSupport(physicalDevice, deviceExtensions) == false)
    return data;

  auto score = GetDeviceScore(data.Properties, data.Features);
  if (score <= 0)
    return data;

  float queuePriority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {
      data.QueueFamilies.ComputeFamily.value(),
      data.QueueFamilies.PresentFamily.value()};

  for (uint32_t queueFamily : uniqueQueueFamilies) {
    auto queueInfo = VkDeviceQueueCreateInfo();
    {
      queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueInfo.queueFamilyIndex = queueFamily;
      queueInfo.queueCount = 1;
      queueInfo.pQueuePriorities = &queuePriority;
    }
    queueCreateInfos.push_back(queueInfo);
  }

  auto deviceFeatures = VkPhysicalDeviceFeatures();
  {
    // todo: device features
  }

  auto deviceInfo = VkDeviceCreateInfo();
  {
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pEnabledFeatures = nullptr;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceInfo.enabledLayerCount = validationLayers.size();
    deviceInfo.ppEnabledLayerNames = validationLayers.data();
    deviceInfo.enabledExtensionCount = deviceExtensions.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  }

  ErrorCheck(vkCreateDevice(physicalDevice, &deviceInfo, nullptr,
                            &data.VirtualDevice));

  // Make sure these queues are supported by the device
  VkBool32 isSupported = false;
  ErrorCheck(vkGetPhysicalDeviceSurfaceSupportKHR(
      physicalDevice, data.QueueFamilies.ComputeFamily.value(), surface,
      &isSupported));
  if (!isSupported) {
    return data;
  }
  ErrorCheck(vkGetPhysicalDeviceSurfaceSupportKHR(
      physicalDevice, data.QueueFamilies.PresentFamily.value(), surface,
      &isSupported));
  if (!isSupported) {
    return data;
  }

  vkGetDeviceQueue(data.VirtualDevice, data.QueueFamilies.ComputeFamily.value(),
                   0, &data.ComputeQueue);
  vkGetDeviceQueue(data.VirtualDevice, data.QueueFamilies.PresentFamily.value(),
                   0, &data.PresentQueue);

  data.IsReady = true;
  return data;
}
void DestroyDevice(VulkanDevice device) {
  vkDestroyDevice(device.VirtualDevice, nullptr);
}
} // namespace Graphics
} // namespace Tortuga