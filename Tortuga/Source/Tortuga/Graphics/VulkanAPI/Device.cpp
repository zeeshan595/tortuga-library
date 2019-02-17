#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice, std::vector<const char *> extensions)
{
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
  for (const auto &extension : availableExtensions)
  {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR &surface)
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

    VkBool32 presentSupport = false;
    if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport) != VK_SUCCESS)
    {
      Console::Fatal("Failed to check device surface support");
    }
    if (presentSupport)
    {
      indices.PresentFamily = i;
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

  //Setup helper window & surface
  auto window = SDL_CreateWindow(
      "Device Helper Window",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      1024, 768,
      SDL_WINDOW_VULKAN);
  VkSurfaceKHR surface;
  if (SDL_Vulkan_CreateSurface(window, vulkan.Instance, &surface) == false)
  {
    Console::Fatal("Failed to setup device helper surface!");
  }

  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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

    auto deviceQueueFamilies = FindQueueFamilies(physicalDevices[i], surface);
    if (QueueFamilyIndices::IsComplete(deviceQueueFamilies) == false)
      continue;

    if (CheckDeviceExtensionSupport(physicalDevices[i], deviceExtensions) == false)
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
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        data[i].QueueFamilies.GraphicsFamily.value(),
        data[i].QueueFamilies.PresentFamily.value()};

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
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
      //todo: Device features
    }

    auto deviceInfo = VkDeviceCreateInfo();
    {
      deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      deviceInfo.pEnabledFeatures = &deviceFeatures;
      deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
      deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
      deviceInfo.enabledLayerCount = vulkan.ValidationLayers.size();
      deviceInfo.ppEnabledLayerNames = vulkan.ValidationLayers.data();
      deviceInfo.enabledExtensionCount = deviceExtensions.size();
      deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    if (vkCreateDevice(data[i].PhysicalDevice, &deviceInfo, nullptr, &data[i].Device) != VK_SUCCESS)
    {
      Console::Fatal("Failed to create device: {0}", data[i].Properties.deviceName);
    }

    vkGetDeviceQueue(data[i].Device, data[i].QueueFamilies.GraphicsFamily.value(), 0, &data[i].GraphicQueue);
    vkGetDeviceQueue(data[i].Device, data[i].QueueFamilies.PresentFamily.value(), 0, &data[i].PresentQueue);
    data[i].Instance = vulkan.Instance;
  }

  //Destroy helper surface & window
  vkDestroySurfaceKHR(vulkan.Instance, surface, nullptr);
  SDL_DestroyWindow(window);

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