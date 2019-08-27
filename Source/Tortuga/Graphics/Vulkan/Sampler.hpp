#ifndef _VULKAN_SAMPLER
#define _VUKAN_SAMPLER

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./Image.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Sampler
{
struct Sampler
{
  VkDevice Device;
  VkSampler Sampler;
};
Sampler Create(Device::Device device);
void Destroy(Sampler data);
} // namespace Sampler
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif