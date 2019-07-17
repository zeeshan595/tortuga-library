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
  VkDevice Device;
  VkFence Fence;
};

Fence Create(Device::Device device);
void Destroy(Fence data);

void ResetFences(std::vector<Fence> data);
void WaitForFences(std::vector<Fence> data, bool waitAll = true, uint32_t timeout = std::numeric_limits<uint32_t>::max());
} // namespace Fence
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif