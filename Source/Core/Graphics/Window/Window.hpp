#ifndef _WINDOW
#define _WINDOW
#define GLFW_INCLUDE_VULKAN

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace Tortuga
{
namespace Core
{
namespace Graphics
{
class Window
{
  private:
  SDL_Window* _window;

  public:
    Window(std::string title, uint32_t width, uint32_t height);
    ~Window();

    VkSurfaceKHR CreateSurface(VkInstance instance);
    std::vector<const char*> GetWindowExtensions();
};
}; // namespace Graphics
}; // namespace Core
}; // namespace Tortuga

#endif