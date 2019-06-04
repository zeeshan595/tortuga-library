#include "./VulkanBuffer.h"

namespace Tortuga {
namespace Graphics {

uint32_t MemoryType(VkPhysicalDevice physicalDevice, uint32_t memorySize) {
  VkPhysicalDeviceMemoryProperties properties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &properties);
  uint32_t memoryTypeIndex = VK_MAX_MEMORY_TYPES;

  for (uint32_t k = 0; k < properties.memoryTypeCount; k++) {
    if ((VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT &
         properties.memoryTypes[k].propertyFlags) &&
        (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT &
         properties.memoryTypes[k].propertyFlags) &&
        (memorySize <
         properties.memoryHeaps[properties.memoryTypes[k].heapIndex].size)) {
      memoryTypeIndex = k;
      break;
    }
  }

  return memoryTypeIndex;
}

VulkanBuffer CreateVulkanBuffer(VulkanDevice device, uint32_t bufferSize) {
  auto data = VulkanBuffer();
  data.VirtualDevice = device.VirtualDevice;

  auto bufferInfo = VkBufferCreateInfo();
  {
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto queueFamilies = device.QueueFamilies.Values();
    bufferInfo.queueFamilyIndexCount = queueFamilies.size();
    bufferInfo.pQueueFamilyIndices = queueFamilies.data();
  }
  ErrorCheck(
      vkCreateBuffer(device.VirtualDevice, &bufferInfo, nullptr, &data.Buffer));

  auto memoryInfo = VkMemoryAllocateInfo();
  {
    memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryInfo.allocationSize = bufferSize;
    memoryInfo.memoryTypeIndex = MemoryType(device.PhysicalDevice, bufferSize);
  }
  ErrorCheck(vkAllocateMemory(device.VirtualDevice, &memoryInfo, nullptr,
                              &data.Memory));
  ErrorCheck(vkBindBufferMemory(device.VirtualDevice, data.Buffer, data.Memory, 0));

  return data;
}

void DestroyVulkanBuffer(VulkanBuffer data) {

  vkDestroyBuffer(data.VirtualDevice, data.Buffer, nullptr);
}
} // namespace Graphics
} // namespace Tortuga