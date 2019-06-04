#include "./VulkanBuffer.h"

namespace Tortuga {
namespace Graphics {
uint32_t FindMemoryType(VulkanDevice device, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);
                        
VulkanBuffer CreateVulkanBuffer(VulkanDevice device, uint32_t bufferSize,
                                VkMemoryPropertyFlags memoryProperties) {
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

  vkGetBufferMemoryRequirements(device.VirtualDevice, data.Buffer,
                                &data.MemoryRequirements);

  auto memoryInfo = VkMemoryAllocateInfo();
  {
    memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryInfo.allocationSize = data.MemoryRequirements.size;
    memoryInfo.memoryTypeIndex = FindMemoryType(
        device, data.MemoryRequirements.memoryTypeBits, memoryProperties);
  }
  ErrorCheck(vkAllocateMemory(device.VirtualDevice, &memoryInfo, nullptr,
                              &data.Memory));
  ErrorCheck(
      vkBindBufferMemory(device.VirtualDevice, data.Buffer, data.Memory, 0));

  return data;
}

void DestroyVulkanBuffer(VulkanBuffer data) {
  vkDestroyBuffer(data.VirtualDevice, data.Buffer, nullptr);
  vkFreeMemory(data.VirtualDevice, data.Memory, nullptr);
}
} // namespace Graphics
} // namespace Tortuga