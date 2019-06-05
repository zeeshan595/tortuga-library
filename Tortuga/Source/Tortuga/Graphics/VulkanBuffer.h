#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include "./VulkanDevice.h"
#include "./VulkanErrorHandler.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanBuffer {
  VkMemoryRequirements MemoryRequirements;
  VkBuffer Buffer = VK_NULL_HANDLE;
  VkDeviceMemory Memory = VK_NULL_HANDLE;
  VkDevice VirtualDevice = VK_NULL_HANDLE;
};
VulkanBuffer CreateVulkanBuffer(VulkanDevice device, uint32_t bufferSize,
                                VkMemoryPropertyFlags memoryProperties);
void DestroyVulkanBuffer(VulkanBuffer data);

template <typename T> T GetVulkanBufferData(VulkanBuffer buffer) {
  T data;
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(&data, temp, sizeof(T));
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
  return data;
}

template <typename T> std::vector<T> GetVulkanBufferData(VulkanBuffer buffer, uint32_t arraySize) {
  std::vector<T> data;
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(&data, temp, sizeof(T) * arraySize);
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
  return data;
}

template <typename T> void SetVulkanBufferData(VulkanBuffer buffer, T data) {
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(temp, &data, sizeof(T));
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
}

template <typename T>
void SetVulkanBufferData(VulkanBuffer buffer, std::vector<T> data) {
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(temp, data.data(), data.size() * sizeof(T));
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
}
} // namespace Graphics
} // namespace Tortuga

#endif