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
  VkBuffer _buffer;
  VkDeviceMemory _bufferMemory;

  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);

public:
  Buffer(Device *device, uint32_t bufferSize);
  ~Buffer();

  template <typename T>
  void Fill(std::vector<T> data)
  {
    void *temp;
    vkMapMemory(_device->GetVirtualDevice(), _bufferMemory, 0, _size, NULL, &temp);
    memcpy(temp, data.data(), (sizeof(data) * data.size()));
    vkUnmapMemory(_device->GetVirtualDevice(), _bufferMemory);
  }

  VkBuffer GetBuffer() { return _buffer; }
  Device *GetDevice() { return _device; }
};
}; // namespace Tortuga

#endif