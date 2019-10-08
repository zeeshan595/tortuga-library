#include "./DisplaySurface.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device)
{
  const uint32_t defaultWidth = 800;
  const uint32_t defaultHeight = 600;

  auto data = DisplaySurface();
  data.Instance = instance.Instance;
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  data.Window = glfwCreateWindow(defaultWidth, defaultHeight, "Tortuga", nullptr, nullptr);
  if (data.Window == nullptr)
  {
    Console::Fatal("failed to create window");
    return data;
  }
  Vulkan::ErrorCheck::Callback(glfwCreateWindowSurface(instance.Instance, data.Window, nullptr, &data.Surface));
  data.Swapchain = Vulkan::Swapchain::Create(device, defaultWidth, defaultHeight, data.Surface);
  glfwSetWindowCloseCallback(data.Window, [](GLFWwindow *windw) {
    Core::Input::TriggerWindowClose();
  });
  glfwSetKeyCallback(data.Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    Core::Input::TriggerKeyEvent((Core::Input::KeyCode::KeyCode)key, (Core::Input::KeyAction::KeyAction)action);
  });
  glfwSetMouseButtonCallback(data.Window, [](GLFWwindow *window, int button, int action, int mods) {
    Core::Input::TriggerKeyEvent((Core::Input::KeyCode::KeyCode)button, (Core::Input::KeyAction::KeyAction)action);
  });
  glfwSetCursorPosCallback(data.Window, [](GLFWwindow *window, double x, double y) {
    Core::Input::TriggerMouseEvent(x, y);
  });
  glfwSetScrollCallback(data.Window, [](GLFWwindow *window, double x, double y) {
    Core::Input::TriggerScrollEvent(x, y);
  });
  glfwSetDropCallback(data.Window, [](GLFWwindow *window, int count, const char **paths) {
    std::vector<std::string> data(count);
    for (int i = 0; i < count; i++)
      data[i] = paths[i];
    Core::Input::TriggerFileDrop(data);
  });
  return data;
}
void Destroy(DisplaySurface data)
{
  Vulkan::Swapchain::Destroy(data.Swapchain);
  vkDestroySurfaceKHR(data.Instance, data.Surface, nullptr);
  glfwDestroyWindow(data.Window);
}
std::vector<const char *> GetVulkanExtensions()
{
  uint32_t extensionsCount;
  const auto extensionsArr = glfwGetRequiredInstanceExtensions(&extensionsCount);
  std::vector<const char *> extensions(extensionsCount);
  for (uint32_t i = 0; i < extensionsCount; i++)
    extensions[i] = extensionsArr[i];
  return extensions;
}
void SetTitle(DisplaySurface data, std::string title)
{
  glfwSetWindowTitle(data.Window, title.c_str());
}
void Dispatch(DisplaySurface data)
{
  glfwPollEvents();
}
bool ShouldClose(DisplaySurface data)
{
  return glfwWindowShouldClose(data.Window);
}
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga