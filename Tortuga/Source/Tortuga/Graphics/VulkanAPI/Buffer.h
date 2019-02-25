#ifndef _VULKAN_API_BUFFER
#define _VULKAN_API_BUFFER

#include "DataStructures.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
BufferData CreateBuffer(DeviceData device, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags memoryProperties, uint32_t size);
void DestroyBuffer(BufferData data);

template <typename T>
void UpdateBufferData(BufferData buffer, std::vector<T> data)
{
  void *temp;
  if (vkMapMemory(buffer.Device, buffer.Memory, 0, buffer.BufferSize, VK_NULL_HANDLE, &temp) != VK_SUCCESS)
  {
    Console::Error("Failed to map device memory!");
    return;
  }
  memcpy(temp, data.data(), data.size() * sizeof(T));
  vkUnmapMemory(buffer.Device, buffer.Memory);
}

template <typename T>
void UpdateBufferData(BufferData buffer, T data)
{
  void *temp;
  if (vkMapMemory(buffer.Device, buffer.Memory, 0, buffer.BufferSize, VK_NULL_HANDLE, &temp) != VK_SUCCESS)
  {
    Console::Error("Failed to map device memory!");
    return;
  }
  memcpy(temp, &data, sizeof(data));
  vkUnmapMemory(buffer.Device, buffer.Memory);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif