#ifndef _WINDOW
#define _WINDOW

#include <vector>
#include <vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include "../Console.h"

namespace Tortuga
{
class Window
{
private:
  SDL_Window *_window;

public:
  Window(const char *title, uint32_t width, uint32_t height);
  ~Window();

  VkSurfaceKHR CreateWindowSurface(VkInstance instance);
  std::vector<const char *> GetVulkanInstanceExtensions();
};
}; // namespace Tortuga

#endif