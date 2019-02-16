#include "RenderingEngine.h"

namespace Tortuga
{
namespace Graphics
{
RenderingEngine CreateRenderingEngine()
{
  auto data = RenderingEngine();
  data.Vulkan = VulkanAPI::CreateVulkanInstance();
  auto devices = VulkanAPI::CreateDevices(data.Vulkan);

  data.Devices.resize(devices.size());
  uint32_t mainDevice;
  for (uint32_t i = 0; i < devices.size(); i++)
  {
    data.Devices[i].VulkanDevice = devices[i];

    if (data.Devices[i].VulkanDevice.Score > data.Devices[mainDevice].VulkanDevice.Score)
      mainDevice = i;
  }
  data.MainDeviceIndex = mainDevice;
  return data;
}
void DestroyRenderingEngine(RenderingEngine data)
{
  std::vector<VulkanAPI::DeviceData> devices(data.Devices.size());
  for (uint32_t i = 0; i < devices.size(); i++)
  {
    devices[i] = data.Devices[i].VulkanDevice;
  }

  VulkanAPI::DestroyDevices(devices);
  VulkanAPI::DestroyVulkanInstance(data.Vulkan);
}
}; // namespace Graphics
}; // namespace Tortuga