#include "HardwareController.h"

namespace Tortuga
{
namespace Graphics
{
void UpdateDeviceRenderRatios(HardwareController &data, std::vector<float> ratios)
{
  if (ratios.size() != data.Devices.size())
  {
    Console::Error("Cannot update device render ratios because the array does not match the list of devices.");
    return;
  }
  float totalRatio = 0.0f;
  for (uint32_t i = 0; i < ratios.size(); i++)
    totalRatio += ratios[i];

  if (totalRatio != 1.0f)
  {
    Console::Error("The device ratios should add up to 1.0!");
    return;
  }

  uint32_t offsetX = 0;
  for (uint32_t i = 0; i < data.Devices.size(); i++)
  {
    data.Devices[i].Viewport.Ratio = ratios[i];
    data.Devices[i].Viewport.X = offsetX;
    data.Devices[i].Viewport.Y = 0;
    data.Devices[i].Viewport.Width = ratios[i] * data.FullWidth;
    data.Devices[i].Viewport.Height = data.FullHeight;

    offsetX += data.Devices[i].Viewport.Width;
  }
}

HardwareController CreateHardwareController(RenderingEngine engine, Window window, std::vector<RenderingDevice> devices)
{
  auto data = HardwareController();
  data.FullWidth = window.Width;
  data.FullHeight = window.Height;
  data.VulkanMainDevice = engine.Devices[engine.MainDeviceIndex].VulkanDevice;
  data.VulkanSurfaceFormat = window.VulkanSwapchain.SurfaceFormat.format;

  uint32_t totalScore = 0;
  for (uint32_t i = 0; i < devices.size(); i++)
    totalScore += devices[i].VulkanDevice.Score;

  uint32_t offsetX = 0;
  data.Devices.resize(devices.size());
  for (uint32_t i = 0; i < devices.size(); i++)
  {
    data.Devices[i].VulkanDevice = devices[i].VulkanDevice;
    data.Devices[i].Viewport = DeviceViewport();
    {
      data.Devices[i].Viewport.Ratio = (float)devices[i].VulkanDevice.Score / (float)totalScore;
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