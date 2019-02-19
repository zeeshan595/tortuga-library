#include "RenderPass.h"

namespace Tortuga
{
namespace Graphics
{
RenderPass CreateRenderPass(HardwareController hardware)
{
  auto data = RenderPass();

  data.VulkanRenderPass.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanRenderPass[i] = VulkanAPI::CreateRenderPass(
        hardware.Devices[i].VulkanDevice,
        hardware.VulkanSurfaceFormat);
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