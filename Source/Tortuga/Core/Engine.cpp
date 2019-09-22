#include "./Engine.hpp"

namespace Tortuga
{
namespace Core
{
namespace Engine
{
Graphics::Vulkan::Instance::Instance VulkanInstance = Graphics::Vulkan::Instance::Create();
uint32_t BestDevice = -1;
void UpdateBestDevice()
{
  if (BestDevice != -1)
    return;

  BestDevice = 0;
  for (uint32_t i = 1; i < VulkanInstance.Devices.size(); i++)
  {
    if (VulkanInstance.Devices[i].Score > VulkanInstance.Devices[BestDevice].Score)
      BestDevice = i;
  }
}

Graphics::Vulkan::Instance::Instance GetVulkan()
{
  return VulkanInstance;
}
std::vector<Graphics::Vulkan::Device::Device> GetAllDevices()
{
  return VulkanInstance.Devices;
}
Graphics::Vulkan::Device::Device GetMainDevice()
{
  UpdateBestDevice();
  return VulkanInstance.Devices[BestDevice];
}
uint32_t GetMainDeviceIndex()
{
  UpdateBestDevice();
  return BestDevice;
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga