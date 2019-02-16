#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
WindowData CreateWindow(VulkanAPI::VulkanData vulkan, std::string title, uint32_t width, uint32_t height, WindowType type)
{
  auto data = WindowData();
  data.Instance = vulkan.Instance;

  Uint32 windowFlags = SDL_WINDOW_VULKAN;
  switch (type)
  {
  case WindowType::FullScreen:
    windowFlags |= SDL_WINDOW_FULLSCREEN;
    break;
  case WindowType::ResizeableWindowed:
    windowFlags |= SDL_WINDOW_RESIZABLE;
    break;
  case WindowType::BorderlessWindowed:
    windowFlags |= SDL_WINDOW_BORDERLESS;
    break;
  default:
  case WindowType::Windowed:
    break;
  }

  data.Window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height,
      windowFlags);

  if (SDL_Vulkan_CreateSurface(data.Window, vulkan.Instance, &data.Surface) == false)
  {
    Console::Fatal("Failed to create vulkan surface for a window");
  }

  return data;
}

void DestroyWindow(WindowData data)
{
  vkDestroySurfaceKHR(data.Instance, data.Surface, nullptr);
  SDL_DestroyWindow(data.Window);
}
}; // namespace Graphics
}; // namespace Tortuga