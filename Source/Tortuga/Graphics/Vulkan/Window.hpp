#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../Console.hpp"
#include "./Instance.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Window
{
struct Window
{
	Instance::Instance VulkanInstance;
	GLFWwindow *Window = nullptr;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	uint32_t Width;
	uint32_t Height;
	std::string Title;
};

Window Create(Instance::Instance instance, const char *title, uint32_t width, uint32_t height);
void Destroy(Window data);
bool PollEvents(Window window);
Window ResizeWindow(Window data, uint32_t width, uint32_t height);
} // namespace Window
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif