#include "Window.h"

namespace Tortuga {
namespace Graphics {
Window CreateWindow(VkInstance instance, std::string title) {
  auto data = Window();
  data.VulkanInstance = instance;
  
  data.SDLWindow = SDL_CreateWindow(title.c_str(), 800, 600, SDL_WINDOWPOS_CENTERED,
                   SDL_WINDOWPOS_CENTERED, SDL_WINDOW_VULKAN);

  if (SDL_Vulkan_CreateSurface(data.SDLWindow, instance, &data.WindowSurface) == false) {
    Console::Fatal("Failed to get window surface");
  }

  uint32_t extensionsCount;
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount, nullptr);
  data.RequiredExtensions.resize(extensionsCount);
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount, data.RequiredExtensions.data());

  return data;
}
void DestroyWindow(Window window) {
  vkDestroySurfaceKHR(window.VulkanInstance, window.WindowSurface, nullptr);
  SDL_DestroyWindow(window.SDLWindow);
}
} // namespace Graphics
} // namespace Tortuga