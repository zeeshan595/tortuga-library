#include "FrameBuffers.h"

namespace Tortuga
{
namespace Graphics
{
FrameBuffer CreateFrameBuffers(HardwareController hardware, RenderPass renderPass)
{
  auto data = FrameBuffer();

  data.Images.resize(hardware.Devices.size());
  data.VulkanFrameBuffers.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.Images[i] = VulkanAPI::CreateVulkanImage(
        hardware.Devices[i].VulkanDevice,
        hardware.FullWidth,
        hardware.FullHeight,
        hardware.VulkanSurfaceFormat);

    data.VulkanFrameBuffers[i] = VulkanAPI::CreateFrameBuffers(
        hardware.Devices[i].VulkanDevice,
        renderPass.VulkanRenderPass[i],
        {data.Images[i]},
        hardware.FullWidth,
        hardware.FullHeight);
  }

  return data;
}
void DestroyFrameBuffers(FrameBuffer data)
{
  for (uint32_t i = 0; i < data.VulkanFrameBuffers.size(); i++)
  {
    VulkanAPI::DestroyFrameBuffers(data.VulkanFrameBuffers[i]);
    VulkanAPI::DestroyVulkanImage(data.Images[i]);
  }
}
} // namespace Graphics
} // namespace Tortuga