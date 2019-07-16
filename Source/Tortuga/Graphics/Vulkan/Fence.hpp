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

Fence Create(Device::Device device)
{
  Fence data = {};
  data.Device = device.Device;

  VkFenceCreateInfo fenceInfo = {};
  {
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  }
  ErrorCheck::Callback(vkCreateFence(device.Device, &fenceInfo, nullptr, &data.Fence));
  return data;
}

void Destroy(Fence data)
{
  vkDestroyFence(data.Device, data.Fence, nullptr);
}

void ResetFences(std::vector<Fence> data)
{
  if (data.size() <= 0)
    return;

  VkDevice device = data[0].Device;

  std::vector<VkFence> fences(data.size());
  for (uint32_t i = 0; i < fences.size(); i++)
  {
    if (device != data[i].Device)
    {
      Console::Warning("WaitForFences(): All fences must come from the same device");
      continue;
    }

    fences[i] = data[i].Fence;
  }

  ErrorCheck::Callback(vkResetFences(device, fences.size(), fences.data()));
}

void WaitForFences(std::vector<Fence> data, bool waitAll = true, uint32_t timeout = std::numeric_limits<uint32_t>::max())
{
  if (data.size() <= 0)
    return;

  VkDevice device = data[0].Device;

  std::vector<VkFence> fences(data.size());
  for (uint32_t i = 0; i < fences.size(); i++)
  {
    if (device != data[i].Device)
    {
      Console::Warning("WaitForFences(): All fences must come from the same device");
      continue;
    }

    fences[i] = data[i].Fence;
  }

  ErrorCheck::Callback(vkWaitForFences(device, fences.size(), fences.data(), waitAll, timeout));
}
} // namespace Fence
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif