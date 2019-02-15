#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice)
{
  auto indices = QueueFamilyIndices();

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies)
  {
    if (queueFamily.queueCount <= 0)
      continue;

    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.GraphicsFamily = i;
    }

    if (QueueFamilyIndices::IsComplete(indices))
    {
      break;
    }

    i++;
  }

  return indices;
}

uint32_t GetDeviceScore(VkPhysicalDeviceProperties properties, VkPhysicalDeviceFeatures features)
{
  uint32_t score = 0;

  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    score += 1000;

  score += properties.limits.maxImageDimension2D;

  if (features.geometryShader == false)
    score = 0;

  return score;
}

std::vector<DeviceData> CreateDevices(VulkanData vulkan)
{
  std::vector<DeviceData> data;

  //Get all physical devices
  uint32_t totalDevices;
  vkEnumeratePhysicalDevices(vulkan.Instance, &totalDevices, nullptr);
  std::vector<VkPhysicalDevice> physicalDevices(totalDevices);
  vkEnumeratePhysicalDevices(vulkan.Instance, &totalDevices, physicalDevices.data());

  for (uint32_t i = 0; i < totalDevices; i++)
  {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevices[i], &deviceFeatures);

    auto deviceQueueFamilies = FindQueueFamilies(physicalDevices[i]);
    if (QueueFamilyIndices::IsComplete(deviceQueueFamilies) == false)
      continue;

    auto score = GetDeviceScore(deviceProperties, deviceFeatures);
    if (score <= 0)
      continue;

    auto deviceData = DeviceData();
    {
      deviceData.QueueFamilies = deviceQueueFamilies;
      deviceData.Properties = deviceProperties;
      deviceData.Features = deviceFeatures;
      deviceData.PhysicalDevice = physicalDevices[i];
      deviceData.Device = VK_NULL_HANDLE;
      deviceData.Score = score;
    }
    data.push_back(deviceData);
  }

  float queuePriority = 1.0f;
  for (uint32_t i = 0; i < data.size(); i++)
  {
    auto queueInfo = VkDeviceQueueCreateInfo();
    {
      queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueInfo.queueFamilyIndex = data[i].QueueFamilies.GraphicsFamily.value();
      queueInfo.queueCount = 1;
      queueInfo.pQueuePriorities = &queuePriority;
    }

    auto deviceFeatures = VkPhysicalDeviceFeatures();

    auto deviceInfo = VkDeviceCreateInfo();
    {
      deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      deviceInfo.pQueueCreateInfos = &queueInfo;
      deviceInfo.queueCreateInfoCount = 1;
      deviceInfo.pEnabledFeatures = &deviceFeatures;
      deviceInfo.enabledLayerCount = vulkan.ValidationLayers.size();
      deviceInfo.ppEnabledLayerNames = vulkan.ValidationLayers.data();
    }

    if (vkCreateDevice(data[i].PhysicalDevice, &deviceInfo, nullptr, &data[i].Device) != VK_SUCCESS)
    {
      Console::Fatal("Failed to create device: {0}", data[i].Properties.deviceName);
    }
  }

  return data;
}
void DestroyDevices(std::vector<DeviceData> data)
{
  for (uint32_t i = 0; i < data.size(); i++)
    vkDestroyDevice(data[i].Device, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga