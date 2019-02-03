#ifndef _BUFFER
#define _BUFFER

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "CommandPool.h"
#include "StorageType.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class Buffer
{
public:
  enum BufferType
  {
    Index,
    Vertex,
    Uniform
  };

private:
  BufferType _bufferType;
  StorageType _storageType;
  uint32_t _size;
  Device *_device;

  //RAM
  VkBuffer _stagingBuffer;
  VkDeviceMemory _stagingMemory;

  //GPU Memory
  VkBuffer _deviceBuffer;
  VkDeviceMemory _deviceMemory;

  void CreateBuffer(VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryProperties, VkBuffer &handler, VkDeviceMemory &deviceMemory);
  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);

public:
  Buffer(Device *device, uint32_t bufferSize, BufferType bufferType, StorageType storageType = StorageType::DeviceCopy);
  ~Buffer();

  void CopyToDevice(VkBuffer &host, VkBuffer &device);

  template <typename T>
  void UpdateData(std::vector<T> data)
  {
    if (_storageType == StorageType::DeviceOnly)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _deviceMemory, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, data.data(), (sizeof(data) * data.size()));
      vkUnmapMemory(_device->GetVirtualDevice(), _deviceMemory);
    }
    else if (_storageType == StorageType::DeviceCopy)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _stagingMemory, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, data.data(), (sizeof(data) * data.size()));
      vkUnmapMemory(_device->GetVirtualDevice(), _stagingMemory);
      CopyToDevice(_stagingBuffer, _deviceBuffer);
    }
    else
    {
      Console::Fatal("Used unknown storage type for buffer!");
    }
  }

  template <typename T>
  void UpdateData(T data)
  {
    if (_storageType == StorageType::DeviceOnly)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _deviceMemory, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, &data, (sizeof(data)));
      vkUnmapMemory(_device->GetVirtualDevice(), _deviceMemory);
    }
    else if (_storageType == StorageType::DeviceCopy)
    {
      void *temp;
      vkMapMemory(_device->GetVirtualDevice(), _stagingMemory, 0, _size, VK_NULL_HANDLE, &temp);
      memcpy(temp, &data, (sizeof(data)));
      vkUnmapMemory(_device->GetVirtualDevice(), _stagingMemory);
      CopyToDevice(_stagingBuffer, _deviceBuffer);
    }
    else
    {
      Console::Fatal("Used unknown storage type for buffer!");
    }
  }

  uint32_t GetSize() { return _size; }
  VkBuffer GetBuffer() { return _deviceBuffer; }
  VkDeviceMemory GetMemory() { return _deviceMemory; }
  Device *GetDevice() { return _device; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif