#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
Window CreateWindow(RenderingDevice device, std::string title, uint32_t width, uint32_t height, WindowType type)
{

  auto data = Window();
  data.Width = width;
  data.Height = height;

  Uint32 windowFlags = SDL_WINDOW_VULKAN;
  switch (type)
  {
  default:
  case WindowType::Windowed:
    break;
  case WindowType::ResizeableWindowed:
    windowFlags |= SDL_WINDOW_RESIZABLE;
    break;
  case WindowType::BorderlessWindowed:
    windowFlags |= SDL_WINDOW_BORDERLESS;
    break;
  case WindowType::FullScreen:
    windowFlags |= SDL_WINDOW_FULLSCREEN;
    break;
  }
  data.VulkanWindow = VulkanAPI::CreateWindow(device.VulkanDevice, title, width, height, windowFlags);
  data.VulkanSwapchain = VulkanAPI::CreateSwapchain(
      device.VulkanDevice,
      data.VulkanWindow.Surface.Surface,
      0, 0,
      width, height);

  return data;
}

void DestroyWindow(Window data)
{
  VulkanAPI::DestroySwapchain(data.VulkanSwapchain);
  VulkanAPI::DestroyWindow(data.VulkanWindow);
}
}; // namespace Graphics
}; // namespace Tortuga