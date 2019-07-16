#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include <vulkan/vulkan.h>

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

Buffer Create(Device::Device device, uint32_t bufferSize, VkMemoryPropertyFlags memoryProperties)
{
  Buffer data = {};
  data.Device = device.Device;
  data.Size = bufferSize;

  VkBufferCreateInfo bufferInfo = {};
  {
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }
  ErrorCheck::Callback(vkCreateBuffer(device.Device, &bufferInfo, nullptr, &data.Buffer));

  vkGetBufferMemoryRequirements(device.Device, data.Buffer, &data.MemoryRequirements);

  VkMemoryAllocateInfo allocInfo = {};
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = data.MemoryRequirements.size;
    allocInfo.memoryTypeIndex = Device::FindMemoryType(device, data.MemoryRequirements.memoryTypeBits, memoryProperties);
  }
  ErrorCheck::Callback(vkAllocateMemory(device.Device, &allocInfo, nullptr, &data.Memory));
  ErrorCheck::Callback(vkBindBufferMemory(device.Device, data.Buffer, data.Memory, 0));

  return data;
}

void Destroy(Buffer data)
{
  vkDestroyBuffer(data.Device, data.Buffer, nullptr);
  vkFreeMemory(data.Device, data.Memory, nullptr);
}
} // namespace Buffer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif