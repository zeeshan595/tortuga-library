#ifndef _GRAPHICS_BUFFER
#define _GRAPHICS_BUFFER

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/Buffer.h"

#include "HardwareController.h"

namespace Tortuga
{
namespace Graphics
{
enum BufferType
{
  BUFFER_TYPE_VERTEX,
  BUFFER_TYPE_INDEX,
  BUFFER_TYPE_UNIFORM
};
enum BufferStorageType
{
  BUFFER_STORAGE_FAST,
  BUFFER_STORAGE_ACCESSIBLE
};
struct Buffer
{
  HardwareController Hardware;
  uint32_t BufferSize;
  BufferType Type;
  BufferStorageType Storage;
  std::vector<VulkanAPI::BufferData> StagingVulkanBuffer;
  std::vector<VulkanAPI::BufferData> VulkanBuffer;
};
Buffer CreateBuffer(HardwareController hardware, BufferType type, uint32_t bufferSize, BufferStorageType storage);
void DestroyBuffer(Buffer data);
template <typename T>
void UpdateBufferData(Buffer buffer, std::vector<T> data)
{
  if (data.size() * sizeof(T) > buffer.BufferSize)
  {
    Console::Error("Failed to update buffer because it is smaller than the size of the data!");
    return;
  }

  for (uint32_t i = 0; i < buffer.Hardware.Devices.size(); i++)
  {
    if (buffer.Storage == BUFFER_STORAGE_FAST)
      VulkanAPI::UpdateBufferData(buffer.StagingVulkanBuffer[i], data);
    else if (buffer.Storage == BUFFER_STORAGE_ACCESSIBLE)
      VulkanAPI::UpdateBufferData(buffer.VulkanBuffer[i], data);
  }
}
template <typename T>
void UpdateBufferData(Buffer buffer, T data)
{
  if (sizeof(T) > buffer.BufferSize)
  {
    Console::Error("Failed to update buffer because it is smaller than the size of the data!");
    return;
  }

  for (uint32_t i = 0; i < buffer.Hardware.Devices.size(); i++)
  {
    if (buffer.Storage == BUFFER_STORAGE_FAST)
      VulkanAPI::UpdateBufferData(buffer.StagingVulkanBuffer[i], data);
    else if (buffer.Storage == BUFFER_STORAGE_ACCESSIBLE)
      VulkanAPI::UpdateBufferData(buffer.VulkanBuffer[i], data);
  }
}
}; // namespace Graphics
}; // namespace Tortuga

#endif