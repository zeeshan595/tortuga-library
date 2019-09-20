#include "./DisplaySurface.hpp"

#include "./Vulkan/ErrorCheck.hpp"

#if __unix__
#include "vulkan/vulkan_wayland.h"
#endif
#include <future>

namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device)
{
  auto data = Create(instance);
  data.Swapchain = Vulkan::Swapchain::Create(device, 1920, 1080, data.Surface);
  return data;
}
DisplaySurface Create(Vulkan::Instance::Instance instance)
{
  auto data = DisplaySurface();
  data.Instance = instance.Instance;
#if __unix__
  data.Wayland = DisplayServer::Wayland::CreateWayland();
  data.WaylandSurface = DisplayServer::Wayland::CreateSurface(data.Wayland);

  VkWaylandSurfaceCreateInfoKHR createInfo;
  {
    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.flags = 0;
    createInfo.display = data.Wayland.Display;
    createInfo.surface = data.WaylandSurface.Surface;
  }
  Vulkan::ErrorCheck::Callback(vkCreateWaylandSurfaceKHR(instance.Instance, &createInfo, nullptr, &data.Surface));
#endif

  return data;
}
void Destroy(DisplaySurface data)
{
#if __unix__
  DisplayServer::Wayland::DestroySurface(data.WaylandSurface);
  DisplayServer::Wayland::DestroyWayland(data.Wayland);
#endif
  Vulkan::Swapchain::Destroy(data.Swapchain);
  vkDestroySurfaceKHR(data.Instance, data.Surface, nullptr);
}
std::vector<const char *> GetVulkanExtensions()
{
  std::vector<const char *> extensions;
  extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if __unix__
  const auto session = std::getenv("XDG_SESSION_TYPE");
  if (strcmp(session, "wayland") == 0)
    extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
  return extensions;
}
void SetTitle(DisplaySurface data, std::string title)
{
#if __unix__
  DisplayServer::Wayland::SetTitle(data.WaylandSurface, title.c_str());
#endif
}
void Dispatch(DisplaySurface data)
{
#if __unix__
  DisplayServer::Wayland::Dispatch(data.Wayland);
#endif
}
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga