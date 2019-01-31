#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

#include "../Core.h"
#include "../Console.h"
#include "Device.h"

namespace Tortuga
{
class Buffer
{
public:
  enum BufferType
  {
    Vertex,
    Index,
    Uniform
  };
  enum StorageType
  {
    OnlyOnDevice,
    SyncToDevice
  };
  enum MemoryType
  {
    HostMemory,
    DeviceMemory
  };
  struct BufferMemoryObject
  {
    VkBuffer Handler;
    VkDeviceMemory Location;
  };

private:
  BufferType _bufferType;
  StorageType _storageType;
  uint32_t _size;
  Device *_device;

  BufferMemoryObject _hostBuffer;
  BufferMemoryObject _deviceBuffer;

  BufferMemoryObject CreateBuffer(VkBufferUsageFlags bufferFlags, VkMemoryPropertyFlags memoryFlags);
  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);
  VkBufferUsageFlags GetBufferFlags(BufferType type, StorageType storage);
  VkMemoryPropertyFlags GetMemoryFlags(MemoryType type);
  void CopyToDevice();

public:
  Buffer(Device *device, uint32_t bufferSize, BufferType type, StorageType storage = StorageType::SyncToDevice);
  ~Buffer();

  template <typename T>
  void UpdateBufferData(std::vector<T> data, bool copyToDevice = true)
  {
    if (_storageType == StorageType::OnlyOnDevice)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _deviceBuffer.Location, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, data.data(), (sizeof(data) * data.size()));
      vkUnmapMemory(_device->GetVirtualDevice(), _deviceBuffer.Location);
    }
    else if (_storageType == StorageType::SyncToDevice)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _hostBuffer.Location, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, data.data(), (sizeof(data) * data.size()));
      vkUnmapMemory(_device->GetVirtualDevice(), _hostBuffer.Location);

      if (copyToDevice)
        CopyToDevice();
    }
    else
    {
      Console::Error("Unknown storage type, failed to update buffer!");
    }
  }

  BufferMemoryObject GetBuffer() { return _deviceBuffer; }
  Device *GetDevice() { return _device; }
};
}; // namespace Tortuga

#endif