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
struct Buffer
{
  HardwareController Hardware;
  uint32_t BufferSize;
  std::vector<VulkanAPI::BufferData> StagingVulkanBuffer;
  std::vector<VulkanAPI::BufferData> VulkanBuffer;
};
Buffer CreateBuffer(HardwareController hardware, BufferType type, uint32_t bufferSize);
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
    VulkanAPI::UpdateBufferData(buffer.StagingVulkanBuffer[i], data);
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
    VulkanAPI::UpdateBufferData(buffer.StagingVulkanBuffer[i], data);
  }
}
}; // namespace Graphics
}; // namespace Tortuga

#endif