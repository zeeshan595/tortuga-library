#ifndef _VULKAN_API_COMMAND_POOL
#define _VULKAN_API_COMMAND_POOL

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
  CommandPool(Device *device, bool canReRecord = false);
  ~CommandPool();

  VkCommandPool GetCommandPool() { return _commandPool; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif