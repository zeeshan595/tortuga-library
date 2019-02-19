#ifndef _COMMAND_POOL
#define _COMMAND_POOL

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/CommandPool.h"

#include "HardwareController.h"

namespace Tortuga
{
namespace Graphics
{
struct CommandPool
{
  std::vector<VulkanAPI::CommandPoolData> VulkanCommandPool;
};
CommandPool CreateCommandPool(HardwareController hardware);
void DestroyCommandPool(CommandPool pool);
}; // namespace Graphics
}; // namespace Tortuga

#endif