#include "./Buffer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Buffer
{
Buffer CreateHost(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
}
Buffer CreateDeviceOnly(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
}
Buffer CreateHostSrc(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
}
Buffer CreateDeviceOnlySrc(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
}
Buffer CreateHostDest(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
}
Buffer CreateDeviceOnlyDest(Device::Device device, uint32_t bufferSize)
{
  return Create(device, bufferSize, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
}
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
  if (data.Buffer == VK_NULL_HANDLE)
    return;
  if (data.Device == VK_NULL_HANDLE)
    return;
  if (data.Memory == VK_NULL_HANDLE)
    return;
  vkDestroyBuffer(data.Device, data.Buffer, nullptr);
  vkFreeMemory(data.Device, data.Memory, nullptr);
}

} // namespace Buffer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga