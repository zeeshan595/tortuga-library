#include "CommandPool.h"

namespace Tortuga
{
namespace Graphics
{
CommandPool CreateCommandPool(HardwareController hardware)
{
  auto data = CommandPool();

  data.VulkanCommandPool.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanCommandPool[i] = VulkanAPI::CreateCommandPool(hardware.Devices[i].VulkanDevice);
  }

  return data;
}
void DestroyCommandPool(CommandPool pool)
{
  for (uint32_t i = 0; i < pool.VulkanCommandPool.size(); i++)
  {
    VulkanAPI::DestroyCommandPool(pool.VulkanCommandPool[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga