#ifndef _VULKAN_API_FENCE
#define _VULKAN_API_FENCE

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class Fence
{
private:
  Device *_device;
  VkFence _fence;

public:
  Fence(Device *device);
  ~Fence();

  VkFence &GetFence() { return _fence; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif