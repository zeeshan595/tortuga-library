#ifndef _GRAPHICS_WINDOW
#define _GRAPHICS_WINDOW

#include "./Vulkan/Swapchain.hpp"

#include "vulkan/vulkan.h"

#if __unix__
#include "./DisplayServer/Wayland/Display.hpp"
#include "./DisplayServer/Wayland/Surface.hpp"
#endif

struct Instance;
struct Device;
namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
struct DisplaySurface
{
  VkInstance Instance;
  VkSurfaceKHR Surface;
  Vulkan::Swapchain::Swapchain Swapchain;
#if __unix__
  DisplayServer::Wayland::Display Wayland;
  DisplayServer::Wayland::Surface WaylandSurface;
#endif
};
DisplaySurface Create(Vulkan::Instance::Instance instance);
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device);
void Destroy(DisplaySurface data);
std::vector<const char*> GetVulkanExtensions();
void SetTitle(DisplaySurface data, std::string title);
struct DisplaySize
{
  uint32_t width;
  uint32_t height;
};
DisplaySize GetDisplaySize(DisplaySurface data);
void Dispatch(DisplaySurface data);
} // namespace Window
} // namespace Graphics
} // namespace Tortuga

#endif