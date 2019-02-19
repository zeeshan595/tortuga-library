#include "HardwareController.h"

namespace Tortuga
{
namespace Graphics
{
HardwareController CreateHardwareController(RenderingEngine engine, Window window)
{
  auto data = HardwareController();
  data.VulkanSurfaceFormat = window.VulkanSwapchain.SurfaceFormat.format;

  uint32_t maxScore = 0;
  for (uint32_t i = 0; i < engine.Devices.size(); i++)
  {
    if (maxScore < engine.Devices[i].VulkanDevice.Score)
      maxScore = engine.Devices[i].VulkanDevice.Score;
  }

  uint32_t offsetX = 0;
  data.Devices.resize(engine.Devices.size());
  for (uint32_t i = 0; i < engine.Devices.size(); i++)
  {
    data.Devices[i].VulkanDevice = engine.Devices[i].VulkanDevice;
    data.Devices[i].Viewport = DeviceViewport();
    {
      data.Devices[i].Viewport.Ratio = engine.Devices[i].VulkanDevice.Score / maxScore;
      data.Devices[i].Viewport.X = offsetX;
      data.Devices[i].Viewport.Y = 0;
      data.Devices[i].Viewport.Width = data.Devices[i].Viewport.Ratio * window.Width;
      data.Devices[i].Viewport.Height = window.Height;
    }

    offsetX += data.Devices[i].Viewport.Width;
  }

  return data;
}
}; // namespace Graphics
}; // namespace Tortuga