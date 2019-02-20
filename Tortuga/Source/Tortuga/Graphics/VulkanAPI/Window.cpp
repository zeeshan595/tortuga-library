#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
WindowData CreateWindow(VulkanAPI::DeviceData device, std::string title, uint32_t width, uint32_t height, Uint32 windowFlags)
{
  auto data = WindowData();
  data.Surface.Instance = device.Instance;
  data.Surface.Device = device.Device;

  data.Window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height,
      windowFlags);

  if (SDL_Vulkan_CreateSurface(data.Window, device.Instance, &data.Surface.Surface) == false)
  {
    Console::Fatal("Failed to create vulkan surface for a window");
  }

  return data;
} // namespace VulkanAPI

void DestroyWindow(WindowData data)
{
  vkDestroySurfaceKHR(data.Surface.Instance, data.Surface.Surface, nullptr);
  SDL_DestroyWindow(data.Window);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga