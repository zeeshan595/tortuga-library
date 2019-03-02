#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
Buffer CreateBuffer(HardwareController hardware, BufferType type, uint32_t bufferSize, BufferStorageType storage)
{
  auto data = Buffer();
  data.Type = type;
  data.Storage = storage;
  data.BufferSize = bufferSize;
  data.Hardware = hardware;

  VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  switch (type)
  {
  case BUFFER_TYPE_VERTEX:
    bufferUsage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    break;
  case BUFFER_TYPE_INDEX:
    bufferUsage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    break;
  case BUFFER_TYPE_UNIFORM:
    bufferUsage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    break;
  }

  if (storage == BUFFER_STORAGE_FAST)
  {
    data.StagingVulkanBuffer.resize(hardware.Devices.size());
    data.VulkanBuffer.resize(hardware.Devices.size());
    for (uint32_t i = 0; i < hardware.Devices.size(); i++)
    {
      data.StagingVulkanBuffer[i] = VulkanAPI::CreateBuffer(
          hardware.Devices[i].VulkanDevice,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
          bufferSize);

      data.VulkanBuffer[i] = VulkanAPI::CreateBuffer(
          hardware.Devices[i].VulkanDevice,
          bufferUsage,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          bufferSize);
    }
  }
  else if (storage == BUFFER_STORAGE_ACCESSIBLE)
  {
    data.VulkanBuffer.resize(hardware.Devices.size());
    for (uint32_t i = 0; i < hardware.Devices.size(); i++)
    {
      data.VulkanBuffer[i] = VulkanAPI::CreateBuffer(
          hardware.Devices[i].VulkanDevice,
          bufferUsage,
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
          bufferSize);
    }
  }
  else
  {
    Console::Error("Failed to create buffer. Storage type is unknown!");
  }

  return data;
}
void DestroyBuffer(Buffer data)
{
  for (uint32_t i = 0; i < data.Hardware.Devices.size(); i++)
  {
    if (data.Storage == BUFFER_STORAGE_FAST)
    {
      VulkanAPI::DestroyBuffer(data.StagingVulkanBuffer[i]);
      VulkanAPI::DestroyBuffer(data.VulkanBuffer[i]);
    }
    else if (data.Storage == BUFFER_STORAGE_ACCESSIBLE)
    {
      VulkanAPI::DestroyBuffer(data.VulkanBuffer[i]);
    }
    else
    {
      Console::Error("Failed to destroy buffer. Storage type is unknown!");
    }
  }
}
}; // namespace Graphics
}; // namespace Tortuga