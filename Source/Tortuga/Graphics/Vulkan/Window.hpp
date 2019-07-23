#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

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
	SDL_Window *Window = nullptr;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	uint32_t Width;
	uint32_t Height;
	bool SignalClose = false;
};

Window Create(Instance::Instance instance, const char *title, uint32_t width, uint32_t height);
void Destroy(Window data);
void PollEvents(Window &window);
} // namespace Window
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif