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
  data.VulkanInstance = instance;
  data.Title = title;
  data.Width = width;
  data.Height = height;

  std::vector<GLFWmonitor *> monitors;
  {
    int monitorCount = 0;
    auto monitorsData = glfwGetMonitors(&monitorCount);
    monitors.resize(monitorCount);
    for (uint32_t i = 0; i < monitors.size(); i++)
      monitors[i] = monitorsData[i];
  }

  if (monitors.size() == 0)
  {
    Console::Fatal("Failed to find a monitor");
  }

  const auto primaryMonitor = monitors[0];
  const auto mode = glfwGetVideoMode(primaryMonitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
  data.Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (data.Window == nullptr)
  {
    Console::Fatal("Failed to create window");
  }
  ErrorCheck::Callback(glfwCreateWindowSurface(instance.Instance, data.Window, nullptr, &data.Surface));

  return data;
}
Window Create(Instance::Instance instance, const char *title)
{
  Window data = {};
  data.VulkanInstance = instance;
  data.Title = title;

  std::vector<GLFWmonitor *> monitors;
  {
    int monitorCount = 0;
    auto monitorsData = glfwGetMonitors(&monitorCount);
    monitors.resize(monitorCount);
    for (uint32_t i = 0; i < monitors.size(); i++)
      monitors[i] = monitorsData[i];
  }

  if (monitors.size() == 0)
  {
    Console::Fatal("Failed to find a monitor");
  }

  const auto primaryMonitor = monitors[0];
  const auto mode = glfwGetVideoMode(primaryMonitor);
  data.Width = mode->width;
  data.Height = mode->height;
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
  data.Window = glfwCreateWindow(mode->width, mode->height, title, primaryMonitor, nullptr);
  if (data.Window == nullptr)
  {
    Console::Fatal("Failed to create window");
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