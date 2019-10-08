#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include <vulkan/vulkan.h>
#include <cstring>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Buffer
{
struct Buffer
{
  VkDevice Device = VK_NULL_HANDLE;
  VkBuffer Buffer = VK_NULL_HANDLE;
  VkMemoryRequirements MemoryRequirements;
  VkDeviceMemory Memory;
  uint32_t Size;
};

Buffer CreateHost(Device::Device device, uint32_t bufferSize, VkBufferUsageFlags usageFlags);
Buffer CreateDevice(Device::Device device, uint32_t bufferSize, VkBufferUsageFlags usageFlags);
Buffer Create(Device::Device device, uint32_t bufferSize, VkMemoryPropertyFlags memoryProperties, VkBufferUsageFlags usageFlags);
void Destroy(Buffer data);

template <typename T>
void GetData(Buffer buffer, T *data, uint32_t size, uint32_t offset = 0)
{
  void *temp;
  ErrorCheck::Callback(vkMapMemory(buffer.Device, buffer.Memory, offset, size, VK_NULL_HANDLE, &temp));
  memcpy(data, temp, size);
  vkUnmapMemory(buffer.Device, buffer.Memory);
}

template <typename T>
void SetData(Buffer buffer, T *data, uint32_t size, uint32_t offset = 0)
{
  void *temp;
  ErrorCheck::Callback(vkMapMemory(buffer.Device, buffer.Memory, offset, size, VK_NULL_HANDLE, &temp));
  memcpy(temp, data, size);
  vkUnmapMemory(buffer.Device, buffer.Memory);
}

} // namespace Buffer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif