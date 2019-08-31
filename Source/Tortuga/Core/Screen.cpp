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
    VulkanWindow = Graphics::Vulkan::Window::Create(Engine::GetVulkan(), title, 1024, 768);
    Swapchain = Graphics::Vulkan::Swapchain::Create(Engine::GetMainDevice(), VulkanWindow);
  }
  ~Window()
  {
    Graphics::Vulkan::Swapchain::Destroy(Swapchain);
    Graphics::Vulkan::Window::Destroy(VulkanWindow);
  }

  void ResizeWindow(uint32_t width, uint32_t height)
  {
    Graphics::Vulkan::Swapchain::Destroy(Swapchain);
    Graphics::Vulkan::Window::Destroy(VulkanWindow);
    VulkanWindow = Graphics::Vulkan::Window::Create(Engine::GetVulkan(), VulkanWindow.Title.c_str(), width, height);
    Swapchain = Graphics::Vulkan::Swapchain::Create(Engine::GetMainDevice(), VulkanWindow);
  }

  void SetFullScreen()
  {
    Graphics::Vulkan::Swapchain::Destroy(Swapchain);
    Graphics::Vulkan::Window::Destroy(VulkanWindow);
    VulkanWindow = Graphics::Vulkan::Window::Create(Engine::GetVulkan(), VulkanWindow.Title.c_str());
    Swapchain = Graphics::Vulkan::Swapchain::Create(Engine::GetMainDevice(), VulkanWindow);
  }
};
auto window = Window("Tortuga", 800, 600);

bool PollEvents()
{
  return Graphics::Vulkan::Window::PollEvents(window.VulkanWindow);
}
void SetWindowTitle(std::string title)
{
  glfwSetWindowTitle(window.VulkanWindow.Window, title.c_str());
  window.Title = title.c_str();
}
void ResizeWindow(uint32_t width, uint32_t height)
{
  window.ResizeWindow(width, height);
}
void SetFullScreen()
{
  window.SetFullScreen();
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
  glfwGetWindowSize(window.VulkanWindow.Window, &width, &height);
  return {width, height};
}
} // namespace Screen
} // namespace Core
} // namespace Tortuga