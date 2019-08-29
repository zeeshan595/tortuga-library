#ifndef _VULKAN_FENCE
#define _VULKAN_FENCE

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Fence
{
struct Fence
{
  VkDevice Device = VK_NULL_HANDLE;
  VkFence Fence = VK_NULL_HANDLE;
};

Fence Create(Device::Device device, bool signaled = false);
void Destroy(Fence data);

void ResetFences(std::vector<Fence> data);
void WaitForFences(std::vector<Fence> data, bool waitAll = true, uint32_t timeout = std::numeric_limits<uint32_t>::max());
bool IsFenceSignaled(Fence data);
} // namespace Fence
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif