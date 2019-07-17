#include "./CommandPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace CommandPool
{
CommandPool Create(Device::Device device, uint32_t queueFamilyIndex)
{
  CommandPool data = {};
  data.Device = device.Device;

  VkCommandPoolCreateInfo poolInfo = {};
  {
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
  }
  ErrorCheck::Callback(vkCreateCommandPool(device.Device, &poolInfo, nullptr, &data.CommandPool));

  return data;
}

void Destroy(CommandPool data)
{
  vkDestroyCommandPool(data.Device, data.CommandPool, nullptr);
}
} // namespace CommandPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga