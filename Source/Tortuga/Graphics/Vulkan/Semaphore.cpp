#include "./Semaphore.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Semaphore
{
Semaphore Create(Device::Device device)
{
  Semaphore data = {};
  data.device = device.Device;

  VkSemaphoreCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  }
  ErrorCheck::Callback(vkCreateSemaphore(device.Device, &createInfo, nullptr, &data.Semaphore));

  return data;
}
void Destroy(Semaphore data)
{
  if (data.Semaphore == VK_NULL_HANDLE)
    return;

  vkDestroySemaphore(data.device, data.Semaphore, nullptr);
}
} // namespace Semaphore
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga