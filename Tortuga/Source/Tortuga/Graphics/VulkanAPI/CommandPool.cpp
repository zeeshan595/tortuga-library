#include "CommandPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
CommandPoolData CreateCommandPool(DeviceData device)
{
  auto data = CommandPoolData();
  data.Device = device.Device;

  auto poolInfo = VkCommandPoolCreateInfo();
  {
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = device.QueueFamilies.GraphicsFamily.value();
  }

  if (vkCreateCommandPool(device.Device, &poolInfo, nullptr, &data.Pool) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create command pool for device: {0}", Console::Arguments() << device.Properties.deviceName);
  }

  return data;
}
void DestroyCommandPool(CommandPoolData data)
{
  vkDestroyCommandPool(data.Device, data.Pool, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga