#include "./Fence.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Fence
{
Fence Create(Device::Device device, bool signaled)
{
  Fence data = {};
  data.Device = device.Device;

  VkFenceCreateInfo fenceInfo = {};
  {
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (signaled)
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
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

void WaitForFences(std::vector<Fence> data, bool waitAll, uint32_t timeout)
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
bool IsFenceSignaled(Fence data)
{
  const auto result = vkGetFenceStatus(data.Device, data.Fence);
  return result == VK_SUCCESS;
}
} // namespace Fence
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga