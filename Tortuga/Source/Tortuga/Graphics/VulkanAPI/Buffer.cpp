#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
BufferData CreateBuffer(DeviceData device, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags memoryProperties, uint32_t size)
{
  auto data = BufferData();
  data.BufferSize = size;
  data.Device = device.Device;

  auto bufferInfo = VkBufferCreateInfo();
  {
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.usage = bufferUsage;
    bufferInfo.size = size;
  }
  if (vkCreateBuffer(device.Device, &bufferInfo, nullptr, &data.Buffer) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create buffer on device: {0}", device.Properties.deviceName);
  }

  VkMemoryRequirements memoryRequirements;
  vkGetBufferMemoryRequirements(device.Device, data.Buffer, &memoryRequirements);

  auto allocateInfo = VkMemoryAllocateInfo();
  {
    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize = memoryRequirements.size;
    allocateInfo.memoryTypeIndex = FindMemoryType(device, memoryRequirements.memoryTypeBits, memoryProperties);
  }
  if (vkAllocateMemory(device.Device, &allocateInfo, nullptr, &data.Memory) != VK_SUCCESS)
  {
    Console::Fatal("Failed to allocate buffer memory for device: {0}", device.Properties.deviceName);
  }

  if (vkBindBufferMemory(device.Device, data.Buffer, data.Memory, 0))
  {
    Console::Fatal("Failed to bind buffer & device memory on device: {0}", device.Properties.deviceName);
  }

  return data;
}
void DestroyBuffer(BufferData data)
{
  vkDestroyBuffer(data.Device, data.Buffer, nullptr);
  vkFreeMemory(data.Device, data.Memory, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga
