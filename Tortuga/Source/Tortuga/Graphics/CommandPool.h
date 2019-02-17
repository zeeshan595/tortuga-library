#ifndef _COMMAND_POOL
#define _COMMAND_POOL

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/CommandPool.h"

#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
struct CommandPool
{
  std::vector<VulkanAPI::CommandPoolData> VulkanCommandPool;
};
CommandPool CreateCommandPool(Window window);
void DestroyCommandPool(CommandPool pool);
}; // namespace Graphics
}; // namespace Tortuga

#endif