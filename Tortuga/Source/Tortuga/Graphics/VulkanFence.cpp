#include "./VulkanFence.h"

namespace Tortuga {
namespace Graphics {
VulkanFence CreateVulkanFence(VulkanDevice device) {
  auto data = VulkanFence();
  data.VirtualDevice = device.VirtualDevice;

  auto fenceInfo = VkFenceCreateInfo();
  {
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = 0;
    fenceInfo.flags = 0;
  }
  ErrorCheck(
      vkCreateFence(data.VirtualDevice, &fenceInfo, nullptr, &data.Fence));

  return data;
}
void DestroyVulkanFence(VulkanFence data) {
  vkDestroyFence(data.VirtualDevice, data.Fence, nullptr);
}
void FenceWait(std::vector<VulkanFence> data, bool waitAll, uint32_t timeout) {
  if (data.size() == 0)
    return;

  std::vector<VkFence> fences(data.size());
  for (uint32_t i = 0; i < data.size(); i++) {
    fences[i] = data[i].Fence;
    if (data[0].VirtualDevice != data[i].VirtualDevice) {
      Console::Error("All fences must be from the same device");
      return;
    }
  }

  ErrorCheck(vkWaitForFences(data[0].VirtualDevice, fences.size(),
                             fences.data(), waitAll, timeout));
}
void FenceReset(std::vector<VulkanFence> data) {
  if (data.size() == 0)
    return;

  std::vector<VkFence> fences(data.size());
  for (uint32_t i = 0; i < data.size(); i++) {
    fences[i] = data[i].Fence;
    if (data[0].VirtualDevice != data[i].VirtualDevice) {
      Console::Error("All fences must be from the same device");
      return;
    }
  }

  ErrorCheck(
      vkResetFences(data[0].VirtualDevice, fences.size(), fences.data()));
}
} // namespace Graphics
} // namespace Tortuga