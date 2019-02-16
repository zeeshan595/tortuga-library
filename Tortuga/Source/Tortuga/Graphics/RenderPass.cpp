#include "RenderPass.h"

namespace Tortuga
{
namespace Graphics
{
RenderPass CreateRenderPass(Window window)
{
  auto data = RenderPass();

  data.VulkanRenderPass.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanRenderPass[i] = VulkanAPI::CreateRenderPass(
        window.VulkanDevicesInUse[i],
        window.VulkanSwapchain[i]);
  }

  return data;
}

void DestroyRenderPass(RenderPass data)
{
  for (uint32_t i = 0; i < data.VulkanRenderPass.size(); i++)
  {
    VulkanAPI::DestroyRenderPass(data.VulkanRenderPass[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga