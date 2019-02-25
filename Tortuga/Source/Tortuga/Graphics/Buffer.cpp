#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
Buffer CreateBuffer(HardwareController hardware, BufferType type, uint32_t bufferSize)
{
  auto data = Buffer();
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

  return data;
}
void DestroyBuffer(Buffer data)
{
  for (uint32_t i = 0; i < data.Hardware.Devices.size(); i++)
  {
    VulkanAPI::DestroyBuffer(data.StagingVulkanBuffer[i]);
    VulkanAPI::DestroyBuffer(data.VulkanBuffer[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga