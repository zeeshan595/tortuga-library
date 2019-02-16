#ifndef _RENDERING_ENGINE
#define _RENDERING_ENGINE

#include "VulkanAPI/Vulkan.h"
#include "VulkanAPI/Device.h"

namespace Tortuga
{
namespace Graphics
{
struct RenderingDevice
{
  VulkanAPI::DeviceData VulkanDevice;
};
struct RenderingEngine
{
  VulkanAPI::VulkanData Vulkan;
  std::vector<RenderingDevice> Devices;
  uint32_t MainDeviceIndex;
};

RenderingEngine CreateRenderingEngine();
void DestroyRenderingEngine(RenderingEngine data);
}; // namespace Graphics
}; // namespace Tortuga

#endif