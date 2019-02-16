#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
WindowData CreateWindow(std::vector<VulkanAPI::DeviceData> devices, std::string title, uint32_t width, uint32_t height, Uint32 windowFlags)
{
  if (devices.size() <= 0)
  {
    Console::Fatal("Cannot create window without a GPU!");
  }

  auto data = WindowData();

  data.Window = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height,
      windowFlags);

  data.Surface.resize(devices.size());
  for (uint32_t i = 0; i < devices.size(); i++)
  {
    data.Surface[i].Device = devices[i].Device;
    data.Surface[i].Instance = devices[i].Instance;
    if (SDL_Vulkan_CreateSurface(data.Window, devices[i].Instance, &data.Surface[i].Surface) == false)
    {
      Console::Fatal("Failed to create vulkan surface for a window");
    }
  }

  return data;
}

void DestroyWindow(WindowData data)
{
  for (uint32_t i = 0; i < data.Surface.size(); i++)
  {
    vkDestroySurfaceKHR(data.Surface[i].Instance, data.Surface[i].Surface, nullptr);
  }
  SDL_DestroyWindow(data.Window);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga