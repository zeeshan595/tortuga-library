#ifndef _WINDOW
#define _WINDOW

#include "../Core.h"
#include "../Console.h"

namespace Tortuga
{
namespace Graphics
{
class Window
{
private:
  SDL_Window *_window;
  uint32_t _width;
  uint32_t _height;
  const char *_title;

public:
  Window(const char *title, uint32_t width, uint32_t height);
  ~Window();

  VkSurfaceKHR CreateWindowSurface(VkInstance instance);
  std::vector<const char *> GetVulkanInstanceExtensions();

  const char *GetTitle() { return _title; }
  uint32_t GetWidth() { return _width; }
  uint32_t GetHeight() { return _height; }
};
}; // namespace Graphics
}; // namespace Tortuga

#endif