#include "Window.h"

namespace Tortuga {
namespace Graphics {
Window CreateVulkanWindow(std::string title, uint32_t width, uint32_t height) {
  auto data = Window();
  data.Width = width;
  data.Height = height;

  //Create Window
  data.SDLWindow = SDL_CreateWindow(
      title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
      SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);

  if (data.SDLWindow == NULL) {
    // In the case that the window could not be made...
    Console::Fatal("Could not create window: {0}", SDL_GetError());
  }

  //Get window extensions being used
  uint32_t extensionsCount;
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount, nullptr);
  data.RequiredExtensions.resize(extensionsCount);
  SDL_Vulkan_GetInstanceExtensions(data.SDLWindow, &extensionsCount,
                                   data.RequiredExtensions.data());

  return data;
}
void DestroyVulkanWindow(Window window) {
  if (window.WindowSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(window.VulkanInstance, window.WindowSurface, nullptr);
  }
  SDL_DestroyWindow(window.SDLWindow);
}

void CreateVulkanSurface(Window &window, VkInstance instance) {
  window.VulkanInstance = instance;
  //Create surface
  if (SDL_Vulkan_CreateSurface(window.SDLWindow, instance,
                               &window.WindowSurface) == false) {
    Console::Fatal("Failed to get window surface");
  }
}
} // namespace Graphics
} // namespace Tortuga