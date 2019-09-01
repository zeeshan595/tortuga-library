#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Device
{
std::vector<DeviceQueueFamilyIndex> DeviceQueueFamilies::GetIndices(DeviceQueueFamilies queueFamilies)
{
  std::vector<DeviceQueueFamilyIndex> data;

  std::vector<DeviceQueueFamilyIndex> families = {queueFamilies.Compute, queueFamilies.Graphics, queueFamilies.Transfer};
  for (uint32_t i = 0; i < families.size(); i++)
  {
    bool duplicate = false;
    for (uint32_t j = 0; j < data.size(); j++)
    {
      if (families[i].Index == data[j].Index)
      {
        duplicate = true;
        break;
      }
    }
    if (!duplicate)
      data.push_back(families[i]);
  }

  if (data.size() == 0)
    data.push_back(queueFamilies.Compute);

  return data;
}

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

  int32_t bestCompute = -1;
  int32_t bestGraphics = -1;
  int32_t bestTransfer = -1;

  int32_t compute = -1;
  int32_t graphics = -1;
  int32_t transfer = -1;

  DeviceQueueFamilies data = {};
  for (uint32_t i = 0; i < queueFamilies.size(); i++)
  {
    auto queueFamily = queueFamilies[i];
    if (queueFamily.queueCount <= 0)
      continue;

    VkBool32 isSupported = false;
    if (surface != VK_NULL_HANDLE)
      ErrorCheck::Callback(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &isSupported));

    if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
      if (!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT))
        bestCompute = i;
      else
        compute = i;

      data.Compute.CanPresent = isSupported;
      data.Compute.Count = queueFamily.queueCount;
    }
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      if (!(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
        bestGraphics = i;
      else
        graphics = i;

      data.Graphics.CanPresent = isSupported;
      data.Graphics.Count = queueFamily.queueCount;
    }
    if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
    {
      if (!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
        bestTransfer = i;
      else
        transfer = i;

      data.Transfer.CanPresent = isSupported;
      data.Transfer.Count = queueFamily.queueCount;
    }
  }

  if (bestCompute > -1)
    data.Compute.Index = bestCompute;
  else if (compute > -1)
    data.Compute.Index = compute;

  if (bestGraphics > -1)
    data.Graphics.Index = bestGraphics;
  else if (graphics > -1)
    data.Graphics.Index = graphics;

  if (bestTransfer > -1)
    data.Transfer.Index = bestTransfer;
  else if (transfer > -1)
    data.Transfer.Index = transfer;

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
  std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME};
  if (!IsExtensionsSupported(physicalDevice, deviceExtensions))
  {
    Console::Warning("Device does not support required extensions: {0}", data.Properties.deviceName);
    return data;
  }

  data.QueueFamilies = FindDeviceQueueIndices(physicalDevice, surface);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  auto familyIndices = DeviceQueueFamilies::GetIndices(data.QueueFamilies);
  std::vector<std::vector<float>> queuePriorities(familyIndices.size());
  for (uint32_t i = 0; i < familyIndices.size(); i++)
  {
    std::vector<float> queuePriority(familyIndices[i].Count);
    for (uint32_t j = 0; queuePriorities[i].size(); j++)
      queuePriority[j] = 1.0f / (float)queuePriority.size();
    queuePriorities[i] = queuePriority;

    VkDeviceQueueCreateInfo queueInfo = {};
    {
      queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueInfo.queueFamilyIndex = familyIndices[i].Index;
      queueInfo.queueCount = familyIndices[i].Count;
      queueInfo.pQueuePriorities = queuePriorities[i].data();
    }
    queueCreateInfos.push_back(queueInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures = {};
  {
    deviceFeatures.samplerAnisotropy = VK_TRUE;
  }

  VkDeviceCreateInfo deviceInfo = {};
  {
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = VK_NULL_HANDLE;
    deviceInfo.pEnabledFeatures = &deviceFeatures;
    deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = nullptr;
    deviceInfo.enabledExtensionCount = deviceExtensions.size();
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  }

  if (data.QueueFamilies.Compute.Count == 0 || data.QueueFamilies.Graphics.Count == 0 || data.QueueFamilies.Transfer.Count == 0)
  {
    Console::Warning("Could not create device queues: {0}", data.Properties.deviceName);
    return data;
  }

  ErrorCheck::Callback(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &data.Device));
  bool presentSetup = false;

  for (uint32_t i = 0; i < data.QueueFamilies.Compute.Count; i++)
  {
    VkQueue queue;
    vkGetDeviceQueue(data.Device, data.QueueFamilies.Compute.Index, i, &queue);
    data.Queues.Compute.push_back(queue);

    if (!presentSetup && data.QueueFamilies.Compute.CanPresent)
    {
      data.Queues.PresentIndex = data.QueueFamilies.Compute.Index;
      data.Queues.Present = data.Queues.Compute[0];
      presentSetup = true;
    }
  }

  for (uint32_t i = 0; i < data.QueueFamilies.Graphics.Count; i++)
  {
    VkQueue queue;
    if (data.QueueFamilies.Graphics.Index == data.QueueFamilies.Compute.Index)
      queue = data.Queues.Compute[i];
    else
      vkGetDeviceQueue(data.Device, data.QueueFamilies.Graphics.Index, i, &queue);

    data.Queues.Graphics.push_back(queue);

    if (!presentSetup && data.QueueFamilies.Graphics.CanPresent)
    {
      data.Queues.PresentIndex = data.QueueFamilies.Graphics.Index;
      data.Queues.Present = data.Queues.Graphics[0];
      presentSetup = true;
    }
  }

  for (uint32_t i = 0; i < data.QueueFamilies.Transfer.Count; i++)
  {
    VkQueue queue;
    if (data.QueueFamilies.Transfer.Index == data.QueueFamilies.Compute.Index)
      queue = data.Queues.Compute[i];
    else if (data.QueueFamilies.Transfer.Index == data.QueueFamilies.Graphics.Index)
      queue = data.Queues.Graphics[i];
    else
      vkGetDeviceQueue(data.Device, data.QueueFamilies.Transfer.Index, i, &queue);

    data.Queues.Transfer.push_back(queue);

    if (!presentSetup && data.QueueFamilies.Transfer.CanPresent)
    {
      data.Queues.PresentIndex = data.QueueFamilies.Transfer.Index;
      data.Queues.Present = data.Queues.Transfer[0];
      presentSetup = true;
    }
  }

  data.CanPresent = data.QueueFamilies.Compute.CanPresent || data.QueueFamilies.Graphics.CanPresent;
  data.IsDeviceCompatible = true;
  return data;
}
void Destroy(Device data)
{
  if (data.Device == VK_NULL_HANDLE)
    return;

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
void WaitForQueue(VkQueue queue)
{
  ErrorCheck::Callback(vkQueueWaitIdle(queue));
}
void WaitForDevice(Device data)
{
  ErrorCheck::Callback(vkDeviceWaitIdle(data.Device));
}
} // namespace Device
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga