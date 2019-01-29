#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include "../Core.h"
#include "../Console.h"
#include "Device.h"

namespace Tortuga
{
class Buffer
{
private:
  uint32_t _size;
  Device *_device;

  //RAM
  VkBuffer _stagingBuffer;
  VkDeviceMemory _stagingMemory;

  //GPU Memory
  VkBuffer _deviceBuffer;
  VkDeviceMemory _deviceMemory;

  void SetupStagingBuffer();
  void SetupDeviceBuffer(VkBufferUsageFlags flag);
  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);

public:
  static const VkBufferUsageFlags VERTEX_BUFFER_FLAGS = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  static const VkBufferUsageFlags INDEX_BUFFER_FLAGS = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

  Buffer(Device *device, uint32_t bufferSize, VkBufferUsageFlags flag);
  ~Buffer();

  void CopyToDevice();

  template <typename T>
  void Fill(std::vector<T> data, bool copyToDevice = true)
  {
    void *temp;
    vkMapMemory(_device->GetVirtualDevice(), _stagingMemory, 0, _size, NULL, &temp);
    memcpy(temp, data.data(), (sizeof(data) * data.size()));
    vkUnmapMemory(_device->GetVirtualDevice(), _stagingMemory);

    if (copyToDevice)
      CopyToDevice();
  }

  VkBuffer GetStagingBuffer() { return _stagingBuffer; }
  VkDeviceMemory GetStagingMemory() { return _stagingMemory; }
  VkBuffer GetDeviceBuffer() { return _deviceBuffer; }
  VkDeviceMemory GetDeviceMemory() { return _deviceMemory; }
  Device *GetDevice() { return _device; }
};
}; // namespace Tortuga

#endif