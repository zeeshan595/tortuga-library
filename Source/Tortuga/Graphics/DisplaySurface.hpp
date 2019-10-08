#ifndef _GRAPHICS_WINDOW
#define _GRAPHICS_WINDOW

#include "./Vulkan/ErrorCheck.hpp"
#include "./Vulkan/Swapchain.hpp"
#include "vulkan/vulkan.h"
#include "../Core/Input.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Instance;
struct Device;
namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
struct DisplaySurface
{
  VkInstance Instance;
  GLFWwindow *Window;
  VkSurfaceKHR Surface;
  Vulkan::Swapchain::Swapchain Swapchain;
};
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device);
void Destroy(DisplaySurface data);
std::vector<const char *> GetVulkanExtensions();
void SetTitle(DisplaySurface data, std::string title);
struct DisplaySize
{
  uint32_t width;
  uint32_t height;
};
DisplaySize GetDisplaySize(DisplaySurface data);
void Dispatch(DisplaySurface data);
bool ShouldClose(DisplaySurface data);
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga

#endif