#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
Window CreateWindow(RenderingEngine engine, std::string title, uint32_t width, uint32_t height, WindowType type)
{
  auto mainDevice = engine.Devices[engine.MainDeviceIndex].VulkanDevice;

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
  data.VulkanWindow = VulkanAPI::CreateWindow(mainDevice, title, width, height, windowFlags);
  data.VulkanSwapchain = VulkanAPI::CreateSwapchain(
      mainDevice,
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