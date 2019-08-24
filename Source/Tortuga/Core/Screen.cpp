#include "./Screen.hpp"

namespace Tortuga
{
namespace Core
{
namespace Screen
{
struct Window
{
  std::string Title;
  Graphics::Vulkan::Window::Window VulkanWindow;
  Graphics::Vulkan::Swapchain::Swapchain Swapchain;

  Window(const char *title, uint32_t width, uint32_t height)
  {
    this->Title = title;
    VulkanWindow = Graphics::Vulkan::Window::Create(Engine::GetVulkan(), title, width, height);
    Swapchain = Graphics::Vulkan::Swapchain::Create(Engine::GetMainDevice(), VulkanWindow);
  }
  ~Window()
  {
    Graphics::Vulkan::Window::Destroy(VulkanWindow);
    Graphics::Vulkan::Swapchain::Destroy(Swapchain);
  }

  void ResizeWindow(uint32_t width, uint32_t height)
  {
    Graphics::Vulkan::Swapchain::Destroy(Swapchain);
    VulkanWindow = Graphics::Vulkan::Window::ResizeWindow(VulkanWindow, width, height);
    Swapchain = Graphics::Vulkan::Swapchain::Create(Engine::GetMainDevice(), VulkanWindow);
  }
};
auto window = Window("Tortuga", 800, 600);

SDL_Event PollEvents()
{
  auto event = Graphics::Vulkan::Window::PollEvents(window.VulkanWindow);
  return event;
}
void SetWindowTitle(std::string title)
{
  SDL_SetWindowTitle(window.VulkanWindow.Window, title.c_str());
  window.Title = title.c_str();
}
void ResizeWindow(uint32_t width, uint32_t height)
{
  window.ResizeWindow(width, height);
  for (uint i = 0; i < OnWindowResize.size(); i++)
  {
    auto ptr = OnWindowResize[i];
    ((void (*)(void *))ptr.func)(ptr.param);
  }
}
Graphics::Vulkan::Window::Window GetWindow()
{
  return window.VulkanWindow;
}
Graphics::Vulkan::Swapchain::Swapchain GetSwapchain()
{
  return window.Swapchain;
}
std::vector<int32_t> GetWindowSize()
{
  int32_t width = 0;
  int32_t height = 0;
  SDL_GetWindowSize(window.VulkanWindow.Window, &width, &height);
  return {width, height};
}
} // namespace Screen
} // namespace Core
} // namespace Tortuga