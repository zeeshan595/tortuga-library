#include "./Engine.hpp"

namespace Tortuga
{
namespace Core
{
namespace Engine
{
Graphics::Vulkan::Instance::Instance VulkanInstance = Graphics::Vulkan::Instance::Create(true);

Graphics::Vulkan::Instance::Instance GetVulkan()
{
  return VulkanInstance;
}
Graphics::Vulkan::Device::Device GetMainDevice()
{
  return VulkanInstance.Devices[0];
}
uint32_t GetMainDeviceIndex()
{
  return 0;
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga