#include "./Buffer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Buffer
{
Buffer Create(Device::Device device, uint32_t bufferSize, VkMemoryPropertyFlags memoryProperties, VkBufferUsageFlags usageFlags)
{
  Buffer data = {};
  data.Device = device.Device;
  data.Size = bufferSize;

  VkBufferCreateInfo bufferInfo = {};
  {
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usageFlags;
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