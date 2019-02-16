#include "FrameBuffers.h"

namespace Tortuga
{
namespace Graphics
{
FrameBuffer CreateFrameBuffers(Window window, RenderPass renderPass)
{
  auto data = FrameBuffer();

  data.VulkanFrameBuffers.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanFrameBuffers[i] = VulkanAPI::CreateFrameBuffers(
        window.VulkanDevicesInUse[i],
        window.VulkanSwapchain[i],
        renderPass.VulkanRenderPass[i]);
  }

  return data;
}
void DestroyFrameBuffers(FrameBuffer data)
{
  for (uint32_t i = 0; i < data.VulkanFrameBuffers.size(); i++)
  {
    VulkanAPI::DestroyFrameBuffers(data.VulkanFrameBuffers[i]);
  }
}
} // namespace Graphics
} // namespace Tortuga