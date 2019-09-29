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