#include "FrameBuffers.h"

namespace Tortuga
{
namespace Graphics
{
FrameBuffer CreateFrameBuffers(HardwareController hardware, RenderPass renderPass)
{
  auto data = FrameBuffer();

  data.VulkanFrameBuffers.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanFrameBuffers[i] = VulkanAPI::CreateFrameBuffers(
        hardware.Devices[i].VulkanDevice,
        renderPass.VulkanRenderPass[i],
        {/* TODO: FRAME BUFFER IMAGES! */},
        hardware.Devices[i].Viewport.Width,
        hardware.Devices[i].Viewport.Height);
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