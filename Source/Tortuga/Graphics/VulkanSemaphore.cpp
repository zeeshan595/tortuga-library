#include "./VulkanSemaphore.h"

namespace Tortuga {
namespace Graphics {
VulkanSemaphore CreateSemaphore(VulkanDevice device) {
  auto data = VulkanSemaphore();
  data.VirtualDevice = device.VirtualDevice;

  auto semaphoreInfo = VkSemaphoreCreateInfo();
  {
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.flags = 0;
    semaphoreInfo.pNext = 0;
  }

  ErrorCheck(vkCreateSemaphore(data.VirtualDevice, &semaphoreInfo, nullptr,
                               &data.Seamphore));

  return data;
}
void DestroySemaphore(VulkanSemaphore data) {
  vkDestroySemaphore(data.VirtualDevice, data.Seamphore, nullptr);
}
} // namespace Graphics
} // namespace Tortuga