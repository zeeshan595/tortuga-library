#ifndef _COMMAND_POOL
#define _COMMAND_POOL

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class CommandPool
{
private:
  Device *_device;
  VkCommandPool _commandPool;

public:
  CommandPool(Device *device);
  ~CommandPool();

  VkCommandPool GetCommandPool() { return _commandPool; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif