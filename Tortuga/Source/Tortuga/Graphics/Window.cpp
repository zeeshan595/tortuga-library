#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
WindowData CreateWindow(std::vector<RenderingDevice> devices, std::string title, uint32_t width, uint32_t height, WindowType type)
{
  if (devices.size() <= 0)
  {
    Console::Fatal("Cannot create a window without a device!");
  }

  auto data = WindowData();

  uint32_t maxDevicesScore = 0;
  data.VulkanDevicesInUse.resize(devices.size());
  for (uint32_t i = 0; i < data.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanDevicesInUse[i] = devices[i].VulkanDevice;
    maxDevicesScore += devices[i].VulkanDevice.Score;
  }

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
  data.VulkanWindow = VulkanAPI::CreateWindow(data.VulkanDevicesInUse, title, width, height, windowFlags);
  data.VulkanSwapchain.resize(data.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < data.VulkanDevicesInUse.size(); i++)
  {
    float ratio = (float)devices[i].VulkanDevice.Score / (float)maxDevicesScore;

    data.VulkanSwapchain[i] = VulkanAPI::CreateSwapchain(
        devices[i].VulkanDevice,
        data.VulkanWindow.Surface[i].Surface,
        width * ratio,
        height);
  }
  return data;
}

void DestroyWindow(WindowData data)
{
  for (uint32_t i = 0; i < data.VulkanSwapchain.size(); i++)
  {
    VulkanAPI::DestroySwapchain(data.VulkanSwapchain[i]);
  }
  VulkanAPI::DestroyWindow(data.VulkanWindow);
}
}; // namespace Graphics
}; // namespace Tortuga