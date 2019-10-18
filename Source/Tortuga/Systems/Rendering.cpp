#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
Rendering::Rendering()
{
  VulkanInstance = Graphics::Vulkan::Instance::Create();
  DisplaySurface = Graphics::DisplaySurface::Create(VulkanInstance, VulkanInstance.Devices[0]);
  
}
Rendering::~Rendering()
{
  Graphics::DisplaySurface::Destroy(DisplaySurface);
  Graphics::Vulkan::Instance::Destroy(VulkanInstance);
}
void Rendering::Update()
{
  Graphics::DisplaySurface::Dispatch(DisplaySurface);
}
} // namespace Systems
} // namespace Tortuga