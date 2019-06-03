#ifndef _WINDOW
#define _WINDOW

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <iostream>
#include <vulkan/vulkan.h>

#include "../Console.h"

namespace Tortuga {
namespace Graphics {
struct Window {
  SDL_Window *SDLWindow;
  VkSurfaceKHR WindowSurface;
  VkInstance VulkanInstance;
  std::vector<const char *> RequiredExtensions;
};

Window CreateWindow(std::string title);
void DestroyWindow(Window window);
Window CreateSurface(Window window, VkInstance instance);
} // namespace Graphics
} // namespace Tortuga

#endif