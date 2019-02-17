#ifndef _VULKAN_API_COMMAND_POOL
#define _VULKAN_API_COMMAND_POOL

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
CommandPoolData CreateCommandPool(DeviceData device);
void DestroyCommandPool(CommandPoolData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif