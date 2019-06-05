#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <iostream>
#include <vulkan/vulkan.h>

#include "../Console.h"

namespace Tortuga {
namespace Graphics {
struct Window {
  uint32_t Width;
  uint32_t Height;
  SDL_Window *SDLWindow;
  VkSurfaceKHR WindowSurface = VK_NULL_HANDLE;
  VkInstance VulkanInstance = VK_NULL_HANDLE;
  std::vector<const char *> RequiredExtensions;
};

Window CreateVulkanWindow(std::string title, uint32_t width, uint32_t height);
void DestroyVulkanWindow(Window window);
void CreateVulkanSurface(Window &window, VkInstance instance);
} // namespace Graphics
} // namespace Tortuga

#endif