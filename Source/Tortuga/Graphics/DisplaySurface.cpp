#include "./DisplaySurface.hpp"

#include "./Vulkan/ErrorCheck.hpp"

#if __unix__
#include "vulkan/vulkan_wayland.h"
#endif

namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device)
{
  auto data = Create(instance);
  data.Swapchain = Vulkan::Swapchain::Create(device, 1027, 768, data.Surface);
  return data;
}
DisplaySurface Create(Vulkan::Instance::Instance instance)
{
  auto data = DisplaySurface();
  data.Instance = instance.Instance;
#if __unix__
  data.Wayland = DisplayServer::Wayland::CreateWayland();
  data.WaylandSurface = DisplayServer::Wayland::CreateSurface(data.Wayland);
  data.WaylandMemoryPool = DisplayServer::Wayland::CreatePool(data.Wayland, 7680, 4320);
  data.WaylandBuffer = DisplayServer::Wayland::CreateBuffer(data.WaylandMemoryPool, 1024, 768);

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
  DisplayServer::Wayland::DestroyBuffer(data.WaylandBuffer);
  DisplayServer::Wayland::DestroyPool(data.WaylandMemoryPool);
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
void HandleEvents(DisplaySurface data)
{
#if __unix__
  DisplayServer::Wayland::HandleEvents(data.Wayland);
#endif
}
void SetTitle(DisplaySurface data, std::string title)
{
#if __unix__
  DisplayServer::Wayland::SetTitle(data.WaylandSurface, title.c_str());
#endif
}
DisplaySize GetDisplaySize(DisplaySurface data)
{
#if __unix__
  return {data.WaylandBuffer.Width, data.WaylandBuffer.Height};
#endif
}
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga