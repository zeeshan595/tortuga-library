#ifndef _WINDOW
#define _WINDOW

#include "../Core.h"
#include "../Console.h"

namespace Tortuga
{
class Window
{
private:
  SDL_Window *_window;
  uint32_t _width;
  uint32_t _height;

public:
  Window(const char *title, uint32_t width, uint32_t height);
  ~Window();

  VkSurfaceKHR CreateWindowSurface(VkInstance instance);
  std::vector<const char *> GetVulkanInstanceExtensions();

  uint32_t GetWidth(){ return _width; }
  uint32_t GetHeight(){ return _height; } 
};
}; // namespace Tortuga

#endif