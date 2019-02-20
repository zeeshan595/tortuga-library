#ifndef _HARDWARE_CONTROLLER
#define _HARDWARE_CONTROLLER

#include "VulkanAPI/Device.h"
#include "RenderingEngine.h"
#include "Window.h"

#include "../Core.h"
#include "../Console.h"

namespace Tortuga
{
namespace Graphics
{
struct DeviceViewport
{
  float Ratio;
  uint32_t X;
  uint32_t Y;
  uint32_t Width;
  uint32_t Height;
};
struct DeviceController
{
  VulkanAPI::DeviceData VulkanDevice;
  DeviceViewport Viewport;
};
struct HardwareController
{
  uint32_t FullWidth;
  uint32_t FullHeight;
  VkFormat VulkanSurfaceFormat;
  std::vector<DeviceController> Devices;
  VulkanAPI::DeviceData VulkanMainDevice;
};

HardwareController CreateHardwareController(RenderingEngine engine, Window window, std::vector<RenderingDevice> devices);
}; // namespace Graphics
}; // namespace Tortuga

#endif