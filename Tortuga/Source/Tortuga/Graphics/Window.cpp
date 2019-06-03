#include "Window.h"

namespace Tortuga {
namespace Graphics {
Window CreateWindow(std::string title) {
  auto data = Window();

  data.SDLWindow = SDL_CreateWindow(
      title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
      SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

  if (data.SDLWindow == NULL) {
    // In the case that the window could not be made...
    Console::Fatal("Could not create window: {0}", SDL_GetError());
  }

  uint32_t extensionsCount;
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount, nullptr);
  data.RequiredExtensions.resize(extensionsCount);
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount,
                                   data.RequiredExtensions.data());

  return data;
}
void DestroyWindow(Window window) {
  if (window.WindowSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(window.VulkanInstance, window.WindowSurface, nullptr);
  }
  SDL_DestroyWindow(window.SDLWindow);
}
Window CreateSurface(Window window, VkInstance instance) {
  window.VulkanInstance = instance;
  if (SDL_Vulkan_CreateSurface(window.SDLWindow, instance,
                               &window.WindowSurface) == false) {
    Console::Fatal("Failed to get window surface");
  }
  return window;
}
} // namespace Graphics
} // namespace Tortuga