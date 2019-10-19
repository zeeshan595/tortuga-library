#include "./DisplaySurface.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device)
{
  const uint32_t defaultWidth = 800;
  const uint32_t defaultHeight = 600;

  auto data = DisplaySurface();
  data.Instance = instance.Instance;

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  data.PlatformSurface = new Surface::WaylandSurface();
#elif VK_USE_PLATFORM_WIN32_KHR
#elif VK_USE_PLATFORM_XCB_KHR
#elif VK_USE_PLATFORM_ANDROID_KHR
  Console::Fatal("This platform does not have any supported display server");
#else
  Console::Fatal("This platform does not have any supported display server");
#endif

  data.Surface = data.PlatformSurface->CreateSurface(instance.Instance);
  data.Swapchain = Vulkan::Swapchain::Create(device, defaultWidth, defaultHeight, data.Surface);

  return data;
}
void Destroy(DisplaySurface data)
{
  Vulkan::Swapchain::Destroy(data.Swapchain);
  vkDestroySurfaceKHR(data.Instance, data.Surface, nullptr);
  delete data.PlatformSurface;
}
std::vector<const char *> GetVulkanExtensions()
{
  return Surface::SurfaceInterface::GetVulkanExtensions();
}
void SetTitle(DisplaySurface data, std::string title)
{
}
void Dispatch(DisplaySurface data)
{
  data.PlatformSurface->Dispatch();
}
bool ShouldClose(DisplaySurface data)
{
  return false;
}
bool HasPresentSupport(VkPhysicalDevice device, uint32_t familyIndex)
{
  return Surface::SurfaceInterface::HasPresentSupport(device, familyIndex);
}
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga