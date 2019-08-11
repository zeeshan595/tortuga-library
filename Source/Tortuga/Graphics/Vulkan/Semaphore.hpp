#ifndef _VULKAN_SEMAPHORE
#define _VULKAN_SEMAPHORE

#include <vulkan/vulkan.h>

#include "./Device.hpp"
#include "./ErrorCheck.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Semaphore
{
struct Semaphore
{
  VkDevice device;
  VkSemaphore Semaphore;
};

Semaphore Create(Device::Device device);
void Destroy(Semaphore data);
} // namespace Semaphore
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif