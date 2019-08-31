#include "./Window.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Window
{
Window Create(Instance::Instance instance, const char *title, uint32_t width, uint32_t height)
{
  Window data = {};
  data.Width = width;
  data.Height = height;
  data.VulkanInstance = instance;
  data.Title = title;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  data.Window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (data.Window == nullptr)
  {
    Console::Fatal("Failed to create window");
    return data;
  }
  ErrorCheck::Callback(glfwCreateWindowSurface(instance.Instance, data.Window, nullptr, &data.Surface));

  return data;
}
void Destroy(Window data)
{
  vkDestroySurfaceKHR(data.VulkanInstance.Instance, data.Surface, nullptr);
  glfwSetWindowShouldClose(data.Window, true);
}

bool PollEvents(Window window)
{
  glfwPollEvents();
  return glfwWindowShouldClose(window.Window);
}
} // namespace Window
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga