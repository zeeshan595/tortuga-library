#include "./Window.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Window
{
Window Create(Instance::Instance instance, const char *title, uint32_t width, uint32_t height)
{
  Window data = {};
  data.Width = width;
  data.Height = height;
  data.VulkanInstance = instance.Instance;

  data.Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
  if (data.Window == nullptr)
    Console::Fatal("Failed to create window");

  if (!SDL_Vulkan_CreateSurface(data.Window, instance.Instance, &data.Surface))
    Console::Fatal("Failed to create window surface");

  return data;
}
void Destroy(Window data)
{
  SDL_DestroyWindow(data.Window);
}

SDL_Event PollEvents(Window window)
{
  SDL_Event event;
  SDL_PollEvent(&event);
  return event;
}
Window ResizeWindow(Window data, uint32_t width, uint32_t height)
{
  SDL_SetWindowSize(data.Window, width, height);
  vkDestroySurfaceKHR(data.VulkanInstance, data.Surface, nullptr);
  if (!SDL_Vulkan_CreateSurface(data.Window, data.VulkanInstance, &data.Surface))
    Console::Fatal("Failed to create window surface");
  data.Width = width;
  data.Height = height;
  return data;
}
} // namespace Window
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga