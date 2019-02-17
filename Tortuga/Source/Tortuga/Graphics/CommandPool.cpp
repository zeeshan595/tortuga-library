#include "CommandPool.h"

namespace Tortuga
{
namespace Graphics
{
CommandPool CreateCommandPool(Window window)
{
  auto data = CommandPool();

  data.VulkanCommandPool.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanCommandPool[i] = VulkanAPI::CreateCommandPool(window.VulkanDevicesInUse[i]);
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