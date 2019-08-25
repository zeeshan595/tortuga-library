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
  VkDevice Device;
  VkBuffer Buffer;
  VkMemoryRequirements MemoryRequirements;
  VkDeviceMemory Memory;
  uint32_t Size;
};

Buffer CreateHost(Device::Device device, uint32_t bufferSize);
Buffer CreateDeviceOnly(Device::Device device, uint32_t bufferSize);
Buffer CreateHostSrc(Device::Device device, uint32_t bufferSize);
Buffer CreateDeviceOnlySrc(Device::Device device, uint32_t bufferSize);
Buffer CreateHostDest(Device::Device device, uint32_t bufferSize);
Buffer CreateDeviceOnlyDest(Device::Device device, uint32_t bufferSize);
Buffer Create(Device::Device device, uint32_t bufferSize, VkMemoryPropertyFlags memoryProperties, VkBufferUsageFlags usageFlags);
void Destroy(Buffer data);

template <typename T>
void GetData(Buffer buffer, T *data, uint32_t size)
{
  void *temp;
  ErrorCheck::Callback(vkMapMemory(buffer.Device, buffer.Memory, 0, buffer.MemoryRequirements.size, VK_NULL_HANDLE, &temp));
  memcpy(data, temp, size);
  vkUnmapMemory(buffer.Device, buffer.Memory);
}

template <typename T>
void SetData(Buffer buffer, T *data, uint32_t size)
{
  void *temp;
  ErrorCheck::Callback(vkMapMemory(buffer.Device, buffer.Memory, 0, buffer.MemoryRequirements.size, VK_NULL_HANDLE, &temp));
  memcpy(temp, data, size);
  vkUnmapMemory(buffer.Device, buffer.Memory);
}

} // namespace Buffer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif