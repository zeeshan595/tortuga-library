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

template <typename T>
T GetVulkanBufferData(VulkanBuffer buffer, uint32_t size = 0) {
  T data;
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  if (size == 0)
    memcpy(&data, temp, sizeof(T));
  else
    memcpy(&data, temp, size);
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
  return data;
}

template <typename T>
void GetVulkanBufferData(VulkanBuffer buffer, T *data, uint32_t size) {
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(data, temp, size);
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
}

template <typename T>
void SetVulkanBufferData(VulkanBuffer buffer, T data, uint32_t size) {
  void *temp;
  ErrorCheck(vkMapMemory(buffer.VirtualDevice, buffer.Memory, 0,
                         buffer.MemoryRequirements.size, VK_NULL_HANDLE,
                         &temp));
  memcpy(temp, &data, size);
  vkUnmapMemory(buffer.VirtualDevice, buffer.Memory);
}
} // namespace Graphics
} // namespace Tortuga

#endif