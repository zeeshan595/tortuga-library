#ifndef _VULKAN_COMMAND_POOL
#define _VULKAN_COMMAND_POOL

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace CommandPool
{
struct CommandPool
{
  VkDevice Device = VK_NULL_HANDLE;
  VkCommandPool CommandPool = VK_NULL_HANDLE;
};

CommandPool Create(Device::Device device, uint32_t queueFamilyIndex);
void Destroy(CommandPool data);

} // namespace CommandPool
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif