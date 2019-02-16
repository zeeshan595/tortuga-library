#ifndef _TORTUGA_WINDOW
#define _TORTUGA_WINDOW

#include "../Core.h"
#include "../Console.h"
#include "VulkanAPI/DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
enum WindowType
{
  FullScreen,
  Windowed,
  ResizeableWindowed,
  BorderlessWindowed
};

struct WindowData
{
  VkInstance Instance;
  SDL_Window *Window;
  VkSurfaceKHR Surface;
};

WindowData CreateWindow(VulkanAPI::VulkanData vulkan, std::string title, uint32_t width, uint32_t height, WindowType type);
void DestroyWindow(WindowData data);
}; // namespace Graphics
}; // namespace Tortuga

#endif