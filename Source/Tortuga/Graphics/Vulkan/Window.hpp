#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>

#include "../../Console.hpp"
#include "./Instance.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Window
{
struct Window
{
  SDL_Window *Window = nullptr;
  VkSurfaceKHR Surface = VK_NULL_HANDLE;
  uint32_t Width;
  uint32_t Height;
  bool SignalClose = false;
};

Window Create(Instance::Instance instance, const char *title, uint32_t width, uint32_t height)
{
  Window data = {};
  data.Width = width;
  data.Height = height;

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

void PollEvents(Window &window)
{
  SDL_Event event;
  if (SDL_PollEvent(&event) > 0)
  {
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      window.SignalClose = true;
  }
}
} // namespace Window
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif